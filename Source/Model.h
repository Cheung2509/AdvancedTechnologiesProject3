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

	virtual void draw(DrawData* drawData) override;

private:
	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

	const std::vector<Texture> loadMaterialTexture(aiMaterial * mat, aiTextureType type, std::string typeName);
	
private:
	std::vector<std::unique_ptr<Mesh>> m_meshes;
	std::string m_directory;
	std::vector<Texture> m_texturesLoaded;
};

const int& loadTexture(const char* path, std::string directory);