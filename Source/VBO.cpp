#include "VBO.h"

#include "DrawData.h"
#include "GameData.h"

void VBO::tick(GameData * gameData)
{
	Component::tick(gameData);
}

void VBO::draw(DrawData * drawData)
{
	m_world = m_entity->getWorld();

	glm::mat4 mvp = drawData->m_camera->getProjection() * drawData->m_camera->getView() *
		m_world;

	m_shader->setUniform4fv("u_MVP", 1, GL_FALSE, mvp);
	m_shader->setUniform4f("u_colour", m_colour.x, m_colour.y, m_colour.z, m_colour.w);

	drawData->m_renderer->draw(*m_va, *m_ib, *m_shader);
}

void VBO::calculateminMax()
{
	//Finding minumum and max for vertices
	for (auto& vertex : m_vertices)
	{
		if (vertex.x < m_min.x)
		{
			m_min.x = vertex.x;
		}
		if (vertex.y < m_min.y)
		{
			m_min.y = vertex.y;
		}
		if (vertex.z < m_min.z)
		{
			m_min.z = vertex.z;
		}

		if (vertex.x > m_max.x)
		{
			m_max.x = vertex.x;
		}
		if (vertex.y > m_max.y)
		{
			m_max.y = vertex.y;
		}
		if (vertex.z > m_max.z)
		{
			m_max.z = vertex.z;
		}
	}
}
