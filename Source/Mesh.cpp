#include "Mesh.h"

#include "DrawData.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

#include <fstream>
#include <sstream>
#include <iostream>


void Mesh::tick(GameData * gameData)
{
	GameObject3D::tick(gameData);
}

void Mesh::draw(DrawData * drawData)
{
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
		// Retrieve texture number (the N in diffuse_textureN)
		std::stringstream ss;
		std::string number;
		std::string name = m_textures[i].m_type;

		number = ss.str();

		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}

	glm::mat4 mvp = drawData->m_camera->getProjection() * drawData->m_camera->getView() *
		m_worldMatrix;
	m_shader->setUniform4fv("u_MVP", 1, GL_FALSE, mvp);


	m_shader->bind();
	// Draw mesh
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//drawData->m_renderer->draw(*m_vertexArray, *m_indexBuffer, *m_shader);

	// Always good practice to set everything back to defaults once configured.
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::initialiseMesh()
{
	// Create buffers/arrays
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &boneBuffer);

	glBindVertexArray(m_VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	

	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, m_normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, m_textureCoords));

	if (!m_bones.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, boneBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBoneData) * m_bones.size(), &m_bones[0], GL_STATIC_DRAW);
		//Vertex boneID
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_INT, GL_FALSE, sizeof(VertexBoneData), (GLvoid *)0);
		//Vertex BoneWeight
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (GLvoid *)offsetof(VertexBoneData, m_weights));
	}
	glBindVertexArray(0);
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures,
			std::unique_ptr<Shader> shader, std::vector<VertexBoneData>& bones, std::vector<BoneInfo>& boneInfo)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;
	m_bones = bones;
	m_boneInfo = boneInfo;

	m_shader = std::move(shader);

	initialiseMesh();
}
