#pragma once

#include <glm/glm.hpp>

#include <assimp/types.h>

#include <string>
#include <array>

struct Vertex
{
	glm::vec3 m_pos;
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
	for(unsigned int i = 0; i < sizeof(v.m_id) / sizeof(v.m_id[0]); i++)
	{
		if(v.m_weights[i] == 0.0f)
		{
			v.m_id[i] = float(id);
			v.m_weights[i] = weight;
			return;
		}
	}
	assert(0);
}
