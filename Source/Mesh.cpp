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

std::vector<std::unique_ptr<Texture>> Mesh::m_texturesLoaded = std::vector<std::unique_ptr<Texture>>();

void Mesh::tick(GameData * gameData)
{
	if(gameData->m_keyboard.keyIsPressed('Z') && !gameData->m_prevKeyboard.keyIsPressed('Z'))
	{
		m_animate = !m_animate;
	}

	if(gameData->m_keyboard.keyIsPressed('X') && !gameData->m_prevKeyboard.keyIsPressed('X'))
	{
		renderType = static_cast<RenderType>(renderType + 1);

		if (renderType == RenderType::RENDER_TYPE_SIZE)
		{
			renderType = RenderType::TEXTURE;
		}
	}

	GameObject3D::tick(gameData);
}

void Mesh::draw(DrawData * drawData)
{
	if(renderType == RenderType::TEXTURE)
	{
		for(unsigned int i = 0; i < m_textures.size(); i++)
		{
			m_textures[i]->bind(i);
		}
	}

	m_shader->bind();

	//Send uniform information to GPU
	m_shader->setUniform1i("u_renderType", static_cast<int>(renderType));
	
	m_shader->setBool("u_useAnimations", m_animate);

	glm::mat4 mvp = drawData->m_camera->getProjection() * drawData->m_camera->getView() *
		m_worldMatrix;
	m_shader->setUniform4fv("u_MVP", 1, GL_FALSE, mvp);

	std::vector<glm::mat4> transforms;
	for(auto& mats : m_boneInfo)
	{
		transforms.push_back(mats.m_finalTransform);
	}

	m_shader->setUniform4fv("u_Bones", transforms.size(), GL_FALSE, *transforms.data());
	
	drawData->m_renderer->draw(*m_VA, *m_IB, *m_shader);

	if(renderType == RenderType::TEXTURE)
	{
		for(unsigned int i = 0; i < m_textures.size(); i++)
		{
			m_textures[i]->unbind();
		}
	}
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
		vertex.m_pos = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		
		if (mesh->HasNormals())	
		{
			vertex.m_normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
		else
		{
			vertex.m_normal = glm::vec3(0.0f);
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

const std::vector<Texture*> Mesh::loadMaterialTexture(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture*> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;

		for (auto& texture : m_texturesLoaded)
		{
			if (texture->getPath() == str.C_Str())
			{
				textures.push_back(texture.get());
				skip = true;

				break;
			}
		}

		if (!skip)
		{
			std::unique_ptr<Texture> texture = std::make_unique<Texture>(str.C_Str(), m_directory , typeName) ;
			m_texturesLoaded.push_back(std::move(texture));

			for(auto& texture : m_texturesLoaded)
			{
				if(texture->getPath() == str.C_Str())
				{
					textures.push_back(texture.get());
					skip = true;

					break;
				}
			}
		}
	}

	return textures;
}

void Mesh::initialiseMesh()
{
	m_VB = std::make_unique<VertexBuffer>(&m_vertices[0], sizeof(Vertex) * m_vertices.size());
	m_IB = std::make_unique<IndexBuffer>(&m_indices[0], m_indices.size());

	VertexBufferLayout layout;
	layout.push<Vertex>(0);

	m_VA = std::make_unique<VertexArray>();
	m_VA->init();
	m_VA->addBuffer(*m_VB, layout);
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
