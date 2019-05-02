#include "Mesh.h"

#include "DrawData.h"
#include "GameData.h"
#include "Helper.h"
#include "ErrorHandler.h"
#include "Vendor/SOIL2/SOIL2.h"

#include "Vendor/glm/gtx/matrix_interpolation.hpp"

#include <fstream>
#include <sstream>
#include <iostream>


void Mesh::tick(GameData * gameData)
{
	GameObject3D::tick(gameData);
}

void Mesh::draw(DrawData * drawData)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		GLCALL(glActiveTexture(GL_TEXTURE0 + i)); // Active proper texture unit before binding

		// And finally bind the texture
		GLCALL(glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id));
	}
	
	glm::mat4 mvp = drawData->m_camera->getProjection() * drawData->m_camera->getView() *
		m_worldMatrix;
	m_shader->setUniform4fv("u_MVP", 1, GL_FALSE, mvp);

	for (unsigned int i = 0; i < m_boneInfo.size(); i++)
	{
		m_shader->setUniform4fv("u_Bones[" + std::to_string(i) + "]", 1, GL_FALSE, m_boneInfo[i].m_finalTransform);
	}

	// Draw mesh
	m_shader->bind();
	GLCALL(glBindVertexArray(m_VAO));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCALL(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0));
	
	//drawData->m_renderer->draw(*m_vertexArray, *m_indexBuffer, *m_shader);
	
	GLCALL(glBindVertexArray(0));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	// Always good practice to set everything back to defaults once configured.
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::processMesh(aiMesh* mesh, const aiScene * scene)
{
	m_vertices.reserve(mesh->mNumVertices);
	loadVertices(mesh);

	//Process all indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			m_indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	//Load bones if there are bones in mesh
	if (mesh->HasBones())
	{
		m_bones.reserve(m_vertices.size());
		for (unsigned int i = 0; i < m_vertices.size(); i++)
		{
			m_bones.emplace_back(VertexBoneData());
		}
		m_boneInfo.reserve(mesh->mNumBones);
		loadBones(mesh);
	}

	//Process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		auto diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE,"texture_diffuse");
		m_textures.insert(m_textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		auto specularMaps = loadMaterialTexture(material,aiTextureType_SPECULAR, "texture_specular");
		m_textures.insert(m_textures.end(), specularMaps.begin(), specularMaps.end());
	}
}

void Mesh::loadVertices(aiMesh * mesh)
{
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		//get position
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.m_pos = vector;

		// get normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.m_normal = vector;
		}

		//get texture coord
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 texVec;

			texVec.x = mesh->mTextureCoords[0][i].x;
			texVec.y = mesh->mTextureCoords[0][i].y;
			vertex.m_textureCoords = texVec;
		}
		else
		{
			vertex.m_textureCoords = glm::vec2(0.0f);
		}

		m_vertices.emplace_back(vertex);
	}
}

void Mesh::loadBones(aiMesh * mesh)
{
	unsigned int numBones = 0;
	// Loop through all bones
	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		//Get the bone name
		unsigned int index = 0;
		std::string boneName(mesh->mBones[i]->mName.data);

		//Check if bone name exists alread
		if (m_boneMapping.find(boneName) == m_boneMapping.end())
		{
			//if not found create new boneInfo
			index = numBones;
			numBones++;
			m_boneInfo.emplace_back(BoneInfo());
		}
		else
		{
			//If found set index
			index = m_boneMapping[boneName];
		}

		//Apply it to bone mapping
		m_boneMapping[boneName] = index;
		m_boneInfo[index].m_boneOffset = glm::aiMatrix4x4ToGLM(mesh->mBones[i]->mOffsetMatrix);

		//Loop through all weights
		for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			//Get vertex id and weight and add it to bone data
			unsigned int id = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			m_bones[id].addBoneData(index, weight);
		}
	}
}

const std::vector<Texture> Mesh::loadMaterialTexture(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;

		for (auto& texture : m_texturesLoaded)
		{
			if (texture.m_path == str)
			{
				textures.push_back(texture);
				skip = true;

				break;
			}
		}

		if (!skip)
		{
			Texture texture;
			texture.m_id = loadTexture(str.C_Str(), m_directory);
			texture.m_type = typeName;
			texture.m_path = str;
			textures.push_back(texture);

			m_texturesLoaded.push_back(texture);
		}
	}

	return textures;
}

void Mesh::initialiseMesh()
{
	// Create buffers/arrays
	GLCALL(glGenVertexArrays(1, &m_VAO));
	GLCALL(glGenBuffers(1, &m_VBO));
	GLCALL(glGenBuffers(1, &EBO));
	GLCALL(glGenBuffers(1, &boneBuffer));

	GLCALL(glBindVertexArray(m_VAO));


	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW));
	
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW));

	// Set the vertex attribute pointers
	// Vertex Positions
	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0));
	// Vertex Normals
	GLCALL(glEnableVertexAttribArray(1));
	GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, m_normal)));
	// Vertex Texture Coords
	GLCALL(glEnableVertexAttribArray(2));
	GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, m_textureCoords)));

	if (!m_bones.empty())
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, boneBuffer));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBoneData) * m_bones.size(), &m_bones[0], GL_STATIC_DRAW));
		//Vertex boneID
		GLCALL(glEnableVertexAttribArray(3));
		GLCALL(glVertexAttribPointer(3, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(VertexBoneData), (GLvoid *)0));
		//Vertex BoneWeight
		GLCALL(glEnableVertexAttribArray(4));
		GLCALL(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (GLvoid *)offsetof(VertexBoneData, m_weights)));
	}

	glBindVertexArray(0);
}

void Mesh::setBoneTransform(const glm::mat4 & transform, const std::string name, 
							const glm::mat4& inverseTransform)
{
	if (m_boneMapping.find(name) != m_boneMapping.end())
	{
		m_boneInfo[m_boneMapping[name]].m_finalTransform = inverseTransform * transform *
			m_boneInfo[m_boneMapping[name]].m_boneOffset;
	}
}

const glm::vec3 Mesh::calcInterpolatedScaling(const float & animTime, const aiNodeAnim * node)
{
	if (node->mNumScalingKeys == 1)
	{
		return glm::aiVec3ToGLM(node->mScalingKeys[0].mValue);
	}

	unsigned int scalingIndex = 0;
	assert(node->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < node->mNumScalingKeys - 1; i++)
	{
		if (animTime < (float)node->mScalingKeys[i + 1].mTime)
		{
			scalingIndex = i;
			break;
		}
	}

	unsigned int nextScalingIndex = scalingIndex + 1;
	assert(nextScalingIndex < node->mNumScalingKeys);

	float deltaTime = (float)(node->mScalingKeys[nextScalingIndex].mTime - node->mScalingKeys[scalingIndex].mTime);
	float factor = (animTime - (float)node->mScalingKeys[scalingIndex].mTime) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const glm::vec3& start = glm::aiVec3ToGLM(node->mScalingKeys[scalingIndex].mValue);
	const glm::vec3& end = glm::aiVec3ToGLM(node->mScalingKeys[nextScalingIndex].mValue);
	glm::vec3 delta = end - start;
	return (start + factor * delta);
}

const glm::quat Mesh::calcInterpolatedRotation(const float & animTime, const aiNodeAnim * node)
{
	if (node->mNumRotationKeys == 1)
	{
		return glm::aiQuatToGLM(node->mRotationKeys[0].mValue);
	}

	unsigned int rotationIndex;
	assert(node->mNumRotationKeys > 0);
	for (unsigned int i = 0; i < node->mNumRotationKeys - 1; i++)
	{
		if (animTime < (float)node->mRotationKeys[i + 1].mTime)
		{
			rotationIndex = i;
			break;
		}
	}
	unsigned int nextRotationIndex = rotationIndex + 1;
	assert(nextRotationIndex < node->mNumRotationKeys);
	
	float deltaTime = node->mRotationKeys[nextRotationIndex].mTime - node->mRotationKeys[rotationIndex].mTime;
	float factor = (animTime - (float)node->mRotationKeys[rotationIndex].mTime) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const aiQuaternion& start = node->mRotationKeys[rotationIndex].mValue;
	const aiQuaternion& end = node->mRotationKeys[nextRotationIndex].mValue;
	aiQuaternion out;
	aiQuaternion::Interpolate(out, start, end, factor);
	return glm::aiQuatToGLM(out.Normalize());
}

const glm::vec3 Mesh::calcInterpolatedPosition(const float & animTime, const aiNodeAnim * node)
{
	if (node->mNumPositionKeys == 1)
	{
		return glm::aiVec3ToGLM(node->mPositionKeys[0].mValue);
	}

	unsigned int index;
	for (unsigned int i = 0; i < node->mNumPositionKeys - 1; i++)
	{
		if (animTime < (float)node->mPositionKeys[i + 1].mTime)
		{
			index = i;
			break;
		}
	}
	unsigned int nextIndex = index + 1;
	assert(nextIndex < node->mNumPositionKeys);
	float deltaTime = (float)(node->mPositionKeys[nextIndex].mTime - node->mPositionKeys[index].mTime);
	float factor = (animTime - float(node->mPositionKeys[index].mTime)) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const glm::vec3& start = glm::aiVec3ToGLM(node->mPositionKeys[index].mValue);
	const glm::vec3& end = glm::aiVec3ToGLM(node->mPositionKeys[nextIndex].mValue);
	glm::vec3 delta = end - start;
	return (start + factor * delta);
}

Mesh::Mesh(aiMesh* mesh, const aiScene* scene, std::string directory, std::shared_ptr<Shader> shader)
	: m_directory(directory), m_shader(shader)
{
	processMesh(mesh, scene);

	initialiseMesh();
}

const unsigned int loadTexture(const char * path, std::string directory)
{
	std::string fileName = std::string(path);
	fileName = directory + '/' + fileName;
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width;
	int height;

	unsigned char* image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
				 image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}
