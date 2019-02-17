#pragma once

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "GameObject3D.h"

class Model : public GameObject3D
{
public:
	Model() = default;
	Model(const char* path);

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;

	//Setters
	void setPos(const glm::vec3& newPos);
	void setScale(const glm::vec3& newScale);
	void setColour(const glm::vec4& colour);
	void rotate(const float& angle, const glm::vec3& axis);

private:
	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

	const std::vector<Texture> loadMaterialTexture(aiMaterial * mat, aiTextureType type, std::string typeName);
	
private:
	std::vector<std::unique_ptr<Mesh>> m_meshes;
	std::string m_directory;
	std::vector<Texture> m_texturesLoaded;

	std::map<std::string, unsigned int> m_boneMapping;
};

const int& loadTexture(const char* path, std::string directory);