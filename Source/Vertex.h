#pragma once

#include <glm/glm.hpp>

#include <assimp/types.h>

#include <string>
#include <array>

struct Vertex
{
	glm::vec3 m_pos;
	glm::vec3 m_normal;
	glm::vec2 m_textureCoords;
	glm::vec4 m_id = glm::vec4(0);
	glm::vec4 m_weights = glm::vec4(0.0f);
};

struct BoneInfo
{
	glm::mat4 m_boneOffset;
	glm::mat4 m_finalTransform;
};

static void addBoneData(Vertex& v, unsigned int id, float weight)
{
	unsigned int size = sizeof(glm::vec4) / sizeof(v.m_id[0]);
	for(unsigned int i = 0; i < size ; i++)
	{
		if(v.m_weights[i] == 0.0f)
		{
			v.m_id[i] = float(id);
			v.m_weights[i] = weight;
			return;
		}
	}
	//assert(0);
}
