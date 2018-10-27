#include "CollidableCube.h"

void CollidableCube::init(std::unique_ptr<VBO> model)
{
	m_model = std::move(model);
	m_boundingBox.m_origin = m_pos;
	m_boundingBox.m_halfLength = glm::vec3(std::abs(m_model->getMin().x - m_model->getMax().x) / 2,
										   std::abs(m_model->getMin().y - m_model->getMax().y) / 2,
										   std::abs(m_model->getMin().z - m_model->getMax().z) / 2) * m_scale;

	m_boundingBox.m_axes[0] = glm::vec3(1.0f, 0.0f, 0.0f);
	m_boundingBox.m_axes[1] = glm::vec3(0.0f, 1.0f, 0.0f);
	m_boundingBox.m_axes[1] = glm::vec3(0.0f, 0.0f, 1.0f);
}

void CollidableCube::tick(GameData * gameData)
{
	if (m_collided)
	{
		m_colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		m_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}

	OBBobj::tick(gameData);

	m_boundingBox.m_origin = m_pos;
}

void CollidableCube::draw(DrawData * drawData)
{
	//Draw model
	m_model->setWorld(this->m_worldMatrix);
	m_model->setColour(m_colour);
	m_model->draw(drawData);
}
