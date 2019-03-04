#include "Model.h"

#include <iostream>
#include <string>

#include <assimp/matrix4x4.h>
#include "Vendor/SOIL2/SOIL2.h"

#include "Helper.h"
#include "GameData.h"

Model::Model(const char * path) : GameObject3D()
{
	m_shader = std::make_shared<Shader>("Resources/Shaders/VertexShader.glsl",
										"Resources/Shaders/FragmentShader.frag");
	loadModel(path);
}

Model::~Model()
{
}

void Model::tick(GameData * gameData)
{
	for (auto& mesh : m_meshes)
	{
		if (m_scene->HasAnimations())
		{
			boneTransform(gameData);
		}
		mesh->tick(gameData);
	}

	GameObject3D::tick(gameData);
}

void Model::draw(DrawData * drawData)
{
	for (auto& mesh : m_meshes)
	{
		mesh->draw(drawData);
	}
}

void Model::setPos(const glm::vec3 & newPos)
{
	m_pos = newPos;
	for (auto& mesh : m_meshes)
	{
		mesh->setPos(m_pos);
	}
}

void Model::setScale(const glm::vec3 & newScale)
{
	m_scale = newScale;
	for (auto& mesh : m_meshes)
	{
		mesh->setScale(m_pos);
	}
}

void Model::setColour(const glm::vec4 & colour)
{
	m_colour = colour;
	for (auto& mesh : m_meshes)
	{
		mesh->setColour(colour);
	}
}

void Model::rotate(const float & angle, const glm::vec3 & axis)
{
	m_rotation = glm::rotate(m_rotation, angle, axis);
	for (auto& mesh : m_meshes)
	{
		mesh->rotate(angle, axis);
	}
}

void Model::loadModel(const std::string & path)
{
	importer = std::make_unique<Assimp::Importer>();
	m_scene = std::make_unique<aiScene>(*(importer->ReadFile(path, aiProcess_Triangulate |
										aiProcess_GenSmoothNormals |
										aiProcess_FlipUVs |
										aiProcess_JoinIdenticalVertices)));

	if (!m_scene || m_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode)
	{
		std::cout << "ERROR: ASSIMP: " << importer->GetErrorString() << std::endl;
		return;
	}

	m_directory = path.substr(0, path.find_last_of('/'));

	processNode(m_scene->mRootNode, m_scene.get());
}
	

void Model::processNode(aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];


		m_meshes.emplace_back(std::make_unique<Mesh>(mesh, scene, m_directory, m_shader));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void Model::boneTransform(GameData * data)
{
	float ticksPerSecond = (float)m_scene->mAnimations[0]->mTicksPerSecond != 0 ?
		(float)m_scene->mAnimations[0]->mTicksPerSecond : 25.0f;

	float timeInTick = data->m_runTime * ticksPerSecond;
	float animTime = (float)fmod(timeInTick, m_scene->mAnimations[0]->mDuration);

	readNodeHierarchy(animTime, m_scene->mRootNode, glm::mat4(1.0f));
}

void Model::readNodeHierarchy(float animTime, const aiNode * node, const glm::mat4 & parentTransform)
{
	const std::string name = node->mName.data;

	glm::mat4 nodeTransform(glm::aiMatrix4x4ToGLM(node->mTransformation));
	aiNodeAnim* animNode = findAnimNode(m_scene->mAnimations[0], name);

	if (animNode != nullptr)
	{
		glm::vec3 scale = Mesh::calcInterpolatedScaling(animTime, animNode);
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);

		glm::quat rotation = Mesh::calcInterpolatedRotation(animTime, animNode);
		glm::mat4 rotMat(rotation);

		glm::vec3 translation = Mesh::calcInterpolatedPosition(animTime, animNode);
		glm::mat4 transMat = glm::translate(glm::mat4(1.0f), translation);

		nodeTransform = transMat * rotMat* scaleMat;
	}

	glm::mat4 globalTransform = parentTransform * nodeTransform;

	for (auto& mesh : m_meshes)
	{
		mesh->setBoneTransform(globalTransform, name);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		readNodeHierarchy(animTime, node->mChildren[i], globalTransform);
	}
}

aiNodeAnim * Model::findAnimNode(const aiAnimation * anim, const std::string& name)
{
	for (unsigned int i = 0; i < anim->mNumChannels; i++)
	{
		aiNodeAnim* pNodeAnim = anim->mChannels[i];

		if (pNodeAnim)
		{
			if (pNodeAnim->mNodeName.data == name)
			{
				return pNodeAnim;
			}
		}
	}

	return nullptr;
}
