#pragma once

#include <string>
#include <vector>
#include <map>
#include <string>

#include <assimp/mesh.h>

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
		 std::unique_ptr<Shader> shader, std::vector<VertexBoneData>& bones, std::vector<BoneInfo>& boneInfo);

	void draw(DrawData* drawData) override;

private:
	void initialiseMesh();
private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;
	std::vector<VertexBoneData> m_bones;
	std::vector<BoneInfo> m_boneInfo;

	unsigned int m_VAO, m_VBO, EBO;
	unsigned int boneBuffer;

	std::shared_ptr<Shader> m_shader;
};