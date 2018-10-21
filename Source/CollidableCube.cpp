#include "CollidableCube.h"

void CollidableCube::init(std::unique_ptr<VBO> model)
{
	m_model = std::move(model);
	m_boundingBox.m_min = m_model->getMin() * m_scale;
	m_boundingBox.m_max = m_model->getMax() * m_scale;
}

void CollidableCube::tick(GameData * gameData)
{
	if (m_collided)
	{
		m_model->setColour(glm::vec4(1.0f, 0.0f, 0.f, 1.0f));
	}
	else
	{
		m_model->setColour(glm::vec4(0.0f, 1.0f, 0.f, 1.0f));
	}

	GameObject3D::tick(gameData);
}

void CollidableCube::draw(DrawData * drawData)
{
	//Draw model
	m_model->setWorld(this->m_worldMatrix);
	m_model->draw(drawData);
}
