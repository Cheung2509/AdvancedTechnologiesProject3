#include "Mesh.h"

#include "DrawData.h"
#include "GameData.h"
#include "Helper.h"
#include "ErrorHandler.h"
#include "Vendor/SOIL2/SOIL2.h"

#include "Vendor/glm/gtx/matrix_interpolation.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <sstream>
#include <iostream>


void Mesh::tick(GameData * gameData)
{
	if(gameData->m_keyboard.keyIsPressed('Z'))
	{
		m_animate = !m_animate;
	}

	if(gameData->m_keyboard.keyIsPressed('X'))
	{
		m_textured = !m_textured;
	}
	GameObject3D::tick(gameData);
}

void Mesh::draw(DrawData * drawData)
{
	m_shader->bind();
	if(m_textured)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		for(unsigned int i = 0; i < m_textures.size(); i++)
		{
			GLCALL(glActiveTexture(GL_TEXTURE0 + i)); // Active proper texture unit before binding

			// And finally bind the texture
			GLCALL(glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id));
		}
	}

	//Send uniform information to GPU
	m_shader->setBool("u_textured", m_textured);
	
	m_shader->setBool("u_useAnimations", m_animate);

	glm::mat4 mvp = drawData->m_camera->getProjection() * drawData->m_camera->getView() *
		m_worldMatrix;
	m_shader->setUniform4fv("u_MVP", 1, GL_FALSE, mvp);

	for(int i = 0; i < m_boneInfo.size(); i++)
	{
		std::string name = "u_Bones[" + std::to_string(i) + "]";
		m_shader->setUniform4fv(name, 1, GL_FALSE, m_boneInfo[i].m_finalTransform);
	}

	// Draw mesh
	GLCALL(glBindVertexArray(m_VAO));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCALL(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0));
	
	//drawData->m_renderer->draw(*m_vertexArray, *m_indexBuffer, *m_shader);
	
	GLCALL(glBindVertexArray(0));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	if(m_textured)
	{
		// Always good practice to set everything back to defaults once configured.
		for(unsigned int i = 0; i < m_textures.size(); i++)
		{
			GLCALL(glActiveTexture(GL_TEXTURE0 + i));
			GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
		}
	}
	m_shader->unbind();
}

void Mesh::processMesh(aiMesh* mesh, const aiScene * scene)
{
	m_vertices.reserve(mesh->mNumVertices);
	loadVertices(mesh);

	//Process all indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];

		assert(face.mNumIndices == 3);
		m_indices.push_back(face.mIndices[0]);
		m_indices.push_back(face.mIndices[1]);
		m_indices.push_back(face.mIndices[2]);
	}

	//Load bones if there are bones in mesh
	if (mesh->HasBones())
	{
		loadBones(mesh);
	}

	//Process materials

	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		auto diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		m_textures.insert(m_textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		auto specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
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

		m_vertices.push_back(vertex);
	}
}

void Mesh::loadBones(aiMesh * mesh)
{
	m_boneInfo.reserve(mesh->mNumBones);

	unsigned int numBones = 0;
	// Loop through all bones
 	for (unsigned int i = 0; i < mesh->mNumBones; ++i)
	{
		//Get the bone name
		unsigned int index = 0;
		std::string boneName(mesh->mBones[i]->mName.data);

		//Check if bone name exists already
		if (m_boneMapping.find(boneName) == m_boneMapping.end())
		{
			//if not found create new boneInfo
			index = numBones;
			numBones++;
			m_boneInfo.push_back(BoneInfo());

			//Apply it to bone mapping
			aiMatrix4x4 mat = mesh->mBones[i]->mOffsetMatrix;
			m_boneInfo[i].m_boneOffset = glm::aiMatrix4x4ToGLM(mat);
			m_boneMapping[boneName] = index;
		}
		else
		{
			//If found set index
			index = m_boneMapping[boneName];
		}

		//Loop through all weights
		for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
		{
			//Get vertex id and weight and add it to bone data
			unsigned int id = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			addBoneData(m_vertices[id], index, weight);
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

	GLCALL(glBindVertexArray(m_VAO));


	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(m_vertices[0]), &m_vertices[0], GL_STATIC_DRAW));

	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW));

	// Set the vertex attribute pointers
	// Vertex Positions
	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_pos))));
	// Vertex Texture Coords
	GLCALL(glEnableVertexAttribArray(1));
	GLCALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_textureCoords))));

	//Bone Ids 
	GLCALL(glEnableVertexAttribArray(2));
	GLCALL(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_id))));
	//Bone weights
	GLCALL(glEnableVertexAttribArray(3));
	GLCALL(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_weights))));

	glBindVertexArray(0);
}

void Mesh::setBoneTransform(const glm::mat4& transform, const std::string& name, 
							const glm::mat4& inverseTransform)
{
	if (m_boneMapping.find(name) != m_boneMapping.end())
	{
		unsigned int id = m_boneMapping[name];
		glm::mat4 ifinal = inverseTransform * transform * m_boneInfo[id].m_boneOffset;

		m_boneInfo[id].m_finalTransform = ifinal;
	}
}

Mesh::Mesh(aiMesh* mesh, const aiScene* scene, std::string directory)
	: m_directory(directory)
{
	m_shader = std::make_unique<Shader>("Resources/Shaders/VertexShader.vert",
										"Resources/Shaders/FragmentShader.frag");
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
