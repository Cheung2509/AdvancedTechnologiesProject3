#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <assimp/types.h>

#include <string>

struct Vertex
{
	glm::vec3 m_pos;
	glm::vec3 m_normal;
	glm::vec2 m_textureCoords;
};

struct Texture
{
	GLuint m_id;
	std::string m_type;
	aiString m_path;
};

struct VertexBoneData
{
	unsigned int m_id[4];
	float m_weights[4];

	void addBoneData(unsigned int id, float weight)
	{
		for (unsigned int i = 0; i < sizeof(m_id) / sizeof(m_id[0]); i++)
		{
			if (m_weights[i] == 0.0f)
			{
				m_id[i] = id;
				m_weights[i] = weight;
				return;
			}
		}
		assert(0);
	}
};

struct BoneInfo
{
	glm::mat4 m_boneOffset;
	glm::mat4 m_finalTransform;
};