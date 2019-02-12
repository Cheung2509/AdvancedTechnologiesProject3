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
	unsigned int m_boneID;
};

struct Texture
{
	GLuint m_id;
	std::string m_type;
	aiString m_path;
};

struct BoneWeight
{
	unsigned int m_vertexID;
	float weight;
};

#define NUM_BONES_PER_VERTEX 4

struct VertexBoneData
{
	unsigned int m_IDs[NUM_BONES_PER_VERTEX];
	float m_weights[NUM_BONES_PER_VERTEX];

	void addBoneData(unsigned int id, float weight)
	{
		for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++)
		{
			if (m_weights[i] == 0.0)
			{
				m_IDs[i] = id;
				m_weights[i] = weight;
				return;
			}
		}

		// should never get here - more bones than we have space for
		assert(0);
	}
};

struct BoneInfo
{
	aiMatrix4x4 m_boneOffset;
	aiMatrix4x4 m_finalTranform;
};