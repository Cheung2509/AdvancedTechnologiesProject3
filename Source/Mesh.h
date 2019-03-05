#pragma once

#include <string>
#include <vector>
#include <map>
#include <string>

#include "assimp/anim.h"
#include "assimp/scene.h"

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
	Mesh(aiMesh* mesh, const aiScene* scene, std::string directory, std::shared_ptr<Shader> shader);
	
	void tick(GameData* gameData) override;
	void draw(DrawData* drawData) override;
	
	void setBoneTransform(glm::mat4& transform, const std::string name);

	static const glm::vec3 calcInterpolatedScaling(const float& animTime, const aiNodeAnim* node);
	static const glm::quat calcInterpolatedRotation(const float& animTime, const aiNodeAnim* node);
	static const glm::vec3 calcInterpolatedPosition(const float& animTime, const aiNodeAnim* node);
private:
	void processMesh(aiMesh* mesh, const aiScene * scene);
	void loadVertices(aiMesh* mesh);
	void loadBones(aiMesh* mesh);
	const std::vector<Texture> loadMaterialTexture(aiMaterial * mat,
												   aiTextureType type, std::string typeName);

	void initialiseMesh();
private:
	std::string m_directory;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<VertexBoneData> m_bones;
	std::vector<BoneInfo> m_boneInfo;
	std::vector<Texture> m_textures;
	std::vector<Texture> m_texturesLoaded;

	std::map<std::string, unsigned int> m_boneMapping;

	unsigned int m_VAO, m_VBO, EBO;
	unsigned int boneBuffer;

	std::shared_ptr<Shader> m_shader;
};

const unsigned int loadTexture(const char* path, std::string directory);