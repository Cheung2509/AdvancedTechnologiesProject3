#pragma once

#include <memory>

#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "GameObject3D.h"

class VBO : public GameObject3D
{
public:
	VBO() = default;
	virtual ~VBO() = default;

	virtual void init(std::string vertexShader, std::string fragmentShader);
	virtual void init(std::shared_ptr<Shader> shader);

	std::shared_ptr<Shader> getShader()const { return m_shader; }

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;

	const glm::vec3& getMin() const { return m_min; }
	const glm::vec3& getMax() const { return m_max; }
	
	void setWorld(const glm::mat4& world) { m_worldMatrix = world; }
protected:
	void calculateminMax();

	std::unique_ptr<VertexArray> m_va;
	std::unique_ptr<VertexBuffer> m_vb;
	std::unique_ptr<IndexBuffer> m_ib;
	std::shared_ptr<Shader> m_shader;

	//To set Bounding box of object
	std::vector<glm::vec3> m_vertices;

	glm::vec3 m_min;
	glm::vec3 m_max;
};