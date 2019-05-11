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
	glm::uvec4 m_id = glm::uvec4(0);
	glm::vec4 m_weights = glm::vec4(0.0f);

	inline void addBoneData(unsigned int id, float weight)
	{
		for(unsigned int i = 0; i < sizeof(m_id) / sizeof(m_id[0]); i++)
		{
			if(m_weights[i] == 0.0f)
			{
				m_id[i] = id;
				m_weights[i] = weight;
				return;
			}
		}
		assert(0);
	}
};

struct Texture
{
	unsigned int m_id;
	std::string m_type;
	aiString m_path;
};

struct BoneInfo
{
	glm::mat4 m_boneOffset;
	glm::mat4 m_finalTransform;
};