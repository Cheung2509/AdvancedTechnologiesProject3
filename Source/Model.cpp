#include "Model.h"

#include <iostream>
#include <string>

#include <assimp/matrix4x4.h>
#include "Vendor/SOIL2/SOIL2.h"

Model::Model(const char * path) : GameObject3D()
{
	loadModel(path);
}

void Model::draw(DrawData * drawData)
{
	for (auto& mesh : m_meshes)
	{
		mesh->draw(drawData);
	}
}

void Model::loadModel(const std::string & path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR: ASSIMP: " << importer.GetErrorString() << std::endl;
		return;
	}

	m_directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		Mesh* m = processMesh(mesh, scene);
		std::unique_ptr<Mesh> temp(m);
		m_meshes.push_back(std::move(temp));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh* Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	std::vector<VertexBoneData> bones;
	std::vector<BoneInfo> boneInfo;
	std::map<std::string, unsigned int> boneMapping;

	//get all vertices
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
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.m_normal = vector;

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

		vertices.push_back(vertex);
	}

	//Load bones if there are bones in mesh
	if (mesh->HasBones())
	{
		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			unsigned int boneIndex = 0;
			std::string name = mesh->mBones[i]->mName.data;

			if (boneMapping.find(name) == boneMapping.end())
			{
				boneIndex = mesh->mNumBones;
				BoneInfo bi;
				bi.m_boneOffset = mesh->mBones[i]->mOffsetMatrix;
				boneInfo.push_back(bi);
			}
			else
			{
				boneIndex = boneMapping[name];
			}

			boneMapping[name] = boneIndex;
			boneInfo[boneIndex].m_boneOffset = mesh->mBones[i]->mOffsetMatrix;

			for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
			{
				unsigned int vertexID;
				float weight = mesh->mBones[i]->mWeights[j].mWeight;
				bones[vertexID].addBoneData(boneIndex, weight);
			}
		}
	}

	//Process all indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//Proces materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = loadMaterialTexture(material,
																aiTextureType_DIFFUSE,
																"texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTexture(material,
																aiTextureType_SPECULAR,
																"texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	auto shader = std::make_unique<Shader>("Resources/Shaders/VertexShader.glsl",
										"Resources/Shaders/FragmentShader.frag");

	return new Mesh(vertices, indices, textures, std::move(shader));
}

const std::vector<Texture> Model::loadMaterialTexture(aiMaterial * mat, aiTextureType type, std::string typeName)
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

const int & loadTexture(const char * path, std::string directory)
{
	std::string fileName = std::string(path);
	fileName = directory + '/' + fileName;
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width;
	int height;

	const unsigned char* image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

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
