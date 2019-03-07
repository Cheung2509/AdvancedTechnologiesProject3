#pragma once

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
	~Model();

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

	void boneTransform(GameData* data);
	void readNodeHierarchy(float animTime, const aiNode* node, const glm::mat4& parentTransform);
	aiNodeAnim* findAnimNode(const aiAnimation* anim, const std::string& name);
protected:
	std::vector<std::unique_ptr<Mesh>> m_meshes;
	std::string m_directory;

	std::shared_ptr<Shader> m_shader;

	std::unique_ptr<aiScene> m_scene;
	std::unique_ptr<Assimp::Importer> m_importer;

	std::map<std::string, aiNodeAnim*> m_animNodes;

	glm::mat4 m_globalInverseMatrix;
};