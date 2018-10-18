#include "VBO.h"

#include "DrawData.h"
#include "GameData.h"

void VBO::init(std::string vertexShader, std::string fragmentShader)
{
	m_va = std::make_unique<VertexArray>();
	m_va->init();

	m_shader = std::make_shared<Shader>(vertexShader, fragmentShader);
}

void VBO::init(std::shared_ptr<Shader> shader)
{
	m_va = std::make_unique<VertexArray>();
	m_va->init();

	m_shader = shader;
}

void VBO::tick(GameData * gameData)
{
	GameObject3D::tick(gameData);
}

void VBO::draw(DrawData * drawData)
{
	glm::mat4 mvp = drawData->m_camera->getProjection() * drawData->m_camera->getView() *
		m_worldMatrix;

	m_shader->setUniform4fv("u_MVP", 1, GL_FALSE, mvp);

	drawData->m_renderer->draw(*m_va, *m_ib, *m_shader);
}
