#include "Model.h"

#include <iostream>
#include <string>

#include <assimp/matrix4x4.h>
#include "Vendor/SOIL2/SOIL2.h"

#include "spdlog/spdlog.h"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/common.hpp"

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
	if (gameData->m_keyboard.keyIsPressed('E'))
	{
		rotate(1.0f * gameData->m_deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (gameData->m_keyboard.keyIsPressed('Q'))
	{
		rotate(-1.0f * gameData->m_deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	if(gameData->m_keyboard.keyIsPressed('W'))
	{
		rotate(1.0f * gameData->m_deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if(gameData->m_keyboard.keyIsPressed('S'))
	{
		rotate(-1.0f * gameData->m_deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	if(gameData->m_keyboard.keyIsPressed('A'))
	{
		setPos(m_pos + glm::vec3(-5.0f, 0.0f, 0.0f) *gameData->m_deltaTime);
	}
	else if(gameData->m_keyboard.keyIsPressed('D'))
	{
		setPos(m_pos + glm::vec3(5.0f, 0.0f, 0.0f) *gameData->m_deltaTime);
	}

	for (auto& mesh : m_meshes)
	{
		mesh->tick(gameData);
		if (m_scene->HasAnimations())
		{
			boneTransform(gameData);
		}
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
	m_importer = std::make_unique<Assimp::Importer>();
	m_scene = std::make_unique<aiScene>(*(m_importer->ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes)));

	if (!m_scene || m_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode)
	{
		std::cout << "ERROR: ASSIMP: " << m_importer->GetErrorString() << std::endl;
		return;
	}

	m_globalInverseMatrix = glm::inverse(glm::aiMatrix4x4ToGLM(m_scene->mRootNode->mTransformation));

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
	//Get the animation time
	float ticksPerSecond = (float)m_scene->mAnimations[0]->mTicksPerSecond != 0 ?
		(float)m_scene->mAnimations[0]->mTicksPerSecond : 25.0f;

	float timeInTick = data->m_runTime * ticksPerSecond;
	float animTime = glm::fmod(timeInTick, (float)m_scene->mAnimations[0]->mDuration);

	//Now start getting the transforms for the bones
	readNodeHierarchy(animTime, m_scene->mRootNode, glm::mat4(1.0f));
}

void Model::readNodeHierarchy(const float animTime, const aiNode * node, const glm::mat4& parentTransform)
{
	const std::string name = node->mName.data;

	//get the node transform
	glm::mat4 nodeTransform(glm::aiMatrix4x4ToGLM(node->mTransformation));
	//find the animation nodes
	aiNodeAnim* animNode = findAnimNode(m_scene->mAnimations[0], name);

	if (animNode != nullptr)
	{
		glm::mat4 scale = glm::scale(Mesh::calcInterpolatedScaling(animTime, animNode));

		glm::mat4 rotation = glm::mat4_cast(Mesh::calcInterpolatedRotation(animTime, animNode));

		glm::vec3 translation = Mesh::calcInterpolatedPosition(animTime, animNode);

		nodeTransform = glm::translate(translation) * rotation * scale;
	}

	glm::mat4 globalTransform = parentTransform * nodeTransform;

	for (auto& mesh : m_meshes)
	{
		mesh->setBoneTransform(globalTransform, name, m_globalInverseMatrix);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		readNodeHierarchy(animTime, node->mChildren[i], globalTransform);
	}
}

aiNodeAnim * Model::findAnimNode(const aiAnimation * anim, const std::string& name)
{
	//Find if the node has been used once
	if (m_animNodes.find(name) != m_animNodes.end())
	{
		//Returning cached node
		return m_animNodes[name];
	}
	else
	{
		//If not find the node
		for (unsigned int i = 0; i < anim->mNumChannels; i++)
		{
			aiNodeAnim* pNodeAnim = anim->mChannels[i];

			if (pNodeAnim)
			{
				if (pNodeAnim->mNodeName.data == name)
				{
					//Cache the animation node if found
					m_animNodes[name] = pNodeAnim;
					return m_animNodes[name];
				}
			}
		}
	}

	m_animNodes[name] = nullptr;
	return nullptr;
}
