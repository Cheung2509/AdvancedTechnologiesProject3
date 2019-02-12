#pragma once

#include <string>
#include <vector>
#include <map>
#include <string>

#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "GameObject3D.h"
#include "Shader.h"

class Mesh : public GameObject3D
{
public:
	Mesh() {}
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures,
		  std::unique_ptr<Shader> shader);

	void draw(DrawData* drawData) override;

private:
	void initialiseMesh();
private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;
	std::vector<Bone> m_bones;

	unsigned int VAO, VBO, EBO;

	std::shared_ptr<Shader> m_shader;
};