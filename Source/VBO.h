#pragma once

#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ECS.h"

class VBO : public Component
{
public:
	VBO() = default;
	VBO(std::shared_ptr<Shader> shader);
	virtual ~VBO() = default;

	std::shared_ptr<Shader> getShader()const { return m_shader; }

	virtual void tick(GameData* gameData) override;
	virtual void draw(DrawData* drawData) override;

	const glm::vec3& getMin() const { return m_min; }
	const glm::vec3& getMax() const { return m_max; }
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

	glm::mat4 m_world;
	glm::vec4 m_colour;
};