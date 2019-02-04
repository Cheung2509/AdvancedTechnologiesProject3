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