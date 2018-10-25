#include "CollidableCube.h"

void CollidableCube::init(std::unique_ptr<VBO> model)
{
	m_model = std::move(model);
	m_boundingBox.m_min = m_model->getMin() * m_scale;
	m_boundingBox.m_max = m_model->getMax() * m_scale;
}

void CollidableCube::tick(GameData * gameData)
{
	GameObject3D::tick(gameData);
}

void CollidableCube::draw(DrawData * drawData)
{
	//Draw model
	m_model->setWorld(this->m_worldMatrix);
	m_model->setColour(m_colour);
	m_model->draw(drawData);
}
