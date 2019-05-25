#include "Model.h"

#include <iostream>
#include <string>

#include "Vendor/SOIL2/SOIL2.h"

#include "spdlog/spdlog.h"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/common.hpp"

#include "ErrorHandler.h"
#include "Helper.h"
#include "GameData.h"

Model::Model(const char * path) : GameObject3D()
{
	m_shader = std::make_shared<Shader>("Resources/Shaders/VertexShader.vert",
										"Resources/Shaders/FragmentShader.frag");
	loadModel(path);
}

Model::~Model()
{
	if(m_importer)
	{
		delete m_importer;
		m_importer = nullptr;
	}
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
	}
	
	if(m_scene->HasAnimations())
	{
		boneTransform(gameData);
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
	m_importer = new Assimp::Importer();
	m_scene = new aiScene(*m_importer->ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes));

	if (!m_scene || m_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode)
	{
		std::cout << "ERROR: ASSIMP: " << m_importer->GetErrorString() << std::endl;
		return;
	}

	m_globalInverseMatrix = glm::aiMatrix4x4ToGLM(m_scene->mRootNode->mTransformation);
	m_globalInverseMatrix = glm::inverse(m_globalInverseMatrix);

	m_directory = path.substr(0, path.find_last_of('/'));

	processNode(m_scene->mRootNode, m_scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		m_meshes.emplace_back(std::make_unique<Mesh>(mesh, scene, m_directory));
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

void Model::readNodeHierarchy(const float animTime, const aiNode * node, const glm::mat4 parentTransform)
{
	const std::string name = node->mName.data;

	//get the node transform
	aiMatrix4x4 temp = node->mTransformation;
	glm::mat4 nodeTransform = glm::aiMatrix4x4ToGLM(temp);

	//find the animation nodes
	const aiAnimation* animation = m_scene->mAnimations[0];
	const aiNodeAnim* animNode = findAnimNode(animation, name);

	//Calculate a transform for the bone if node is found
	if (animNode)
	{
		aiVector3D scale = calcInterpolatedScaling(animTime, animNode);
		glm::mat4 matScale = glm::scale(glm::aiVec3ToGLM(scale));

		aiQuaternion rotation = calcInterpolatedRotation(animTime, animNode);
		aiMatrix3x3 rot = rotation.GetMatrix();
		glm::mat4 matRotation = glm::aiMatrix3x3ToGLM(rot);

		aiVector3D translation = calcInterpolatedPosition(animTime, animNode);
		glm::mat4 matTranslation = glm::translate(glm::aiVec3ToGLM(translation));

		nodeTransform = matTranslation * matRotation * matScale;
	}

	//Apply transform to create global transform 
	glm::mat4 globalTransform = parentTransform * nodeTransform;

	//Apply to bones in mesh
	for (auto& mesh : m_meshes)
	{
		mesh->setBoneTransform(globalTransform, name, m_globalInverseMatrix);
	}

	//Recurse until all nodes are done
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
			if(pNodeAnim->mNodeName.data == name)
			{
				//Cache the animation node if found
				m_animNodes[name] = pNodeAnim;
				return m_animNodes[name];
			}
		}
	}

	m_animNodes[name] = nullptr;
	return nullptr;
}

const aiVector3D Model::calcInterpolatedScaling(const float & animTime, const aiNodeAnim * node)
{
	if(node->mNumScalingKeys == 1)
	{
		return node->mScalingKeys[0].mValue;
	}

	unsigned int scalingIndex = findScaling(animTime, node);

	unsigned int nextScalingIndex = scalingIndex + 1;
	assert(nextScalingIndex < node->mNumScalingKeys);

	float deltaTime = (float)(node->mScalingKeys[nextScalingIndex].mTime - node->mScalingKeys[scalingIndex].mTime);
	float factor = (animTime - (float)node->mScalingKeys[scalingIndex].mTime) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const aiVector3D& start = node->mScalingKeys[scalingIndex].mValue;
	const aiVector3D& end = node->mScalingKeys[nextScalingIndex].mValue;
	aiVector3D delta = end - start;
	return (start + factor * delta);
}

const aiQuaternion Model::calcInterpolatedRotation(const float & animTime, const aiNodeAnim * node)
{
	if(node->mNumRotationKeys == 1)
	{
		return node->mRotationKeys[0].mValue;
	}

	unsigned int rotationIndex = findRotation(animTime, node);

	unsigned int nextRotationIndex = rotationIndex + 1;
	assert(nextRotationIndex < node->mNumRotationKeys);

	float deltaTime = (float)node->mRotationKeys[nextRotationIndex].mTime - (float)node->mRotationKeys[rotationIndex].mTime;
	float factor = (animTime - (float)node->mRotationKeys[rotationIndex].mTime) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const aiQuaternion& start = node->mRotationKeys[rotationIndex].mValue;
	const aiQuaternion& end = node->mRotationKeys[nextRotationIndex].mValue;
	aiQuaternion out;
	aiQuaternion::Interpolate(out, start, end, factor);
	return out.Normalize();
}

const aiVector3D Model::calcInterpolatedPosition(const float & animTime, const aiNodeAnim * node)
{
	if(node->mNumPositionKeys == 1)
	{
		return node->mPositionKeys[0].mValue;
	}

	unsigned int index = findPosition(animTime, node);

	unsigned int nextIndex = index + 1;
	assert(nextIndex < node->mNumPositionKeys);
	float deltaTime = (float)(node->mPositionKeys[nextIndex].mTime - node->mPositionKeys[index].mTime);
	float factor = (animTime - float(node->mPositionKeys[index].mTime)) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const aiVector3D& start = node->mPositionKeys[index].mValue;
	const aiVector3D& end = node->mPositionKeys[nextIndex].mValue;
	aiVector3D delta = end - start;
	return (start + factor * delta);
}

const unsigned int Model::findScaling(const float & animTime, const aiNodeAnim * node)
{
	assert(node->mNumScalingKeys > 0);

	for(unsigned int i = 0; i < node->mNumScalingKeys - 1; i++)
	{
		if(animTime < (float)node->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

const unsigned int Model::findPosition(const float & animTime, const aiNodeAnim * node)
{
	assert(node->mNumPositionKeys > 0);

	for(unsigned int i = 0; i < node->mNumPositionKeys - 1; i++)
	{
		if(animTime < (float)node->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

const unsigned int Model::findRotation(const float & animTime, const aiNodeAnim * node)
{
	assert(node->mNumRotationKeys > 0);

	for(unsigned int i = 0; i < node->mNumRotationKeys - 1; i++)
	{
		if(animTime < (float)node->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}
