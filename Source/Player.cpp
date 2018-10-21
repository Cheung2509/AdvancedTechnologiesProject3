#include "Player.h"

#include "GameData.h"
#include "Key.h"

void Player::init(std::unique_ptr<VBO> model)
{
	m_model = std::move(model);
	m_boundingBox.m_min = m_model->getMin() * m_scale;
	m_boundingBox.m_max = m_model->getMax() * m_scale;
}

void Player::tick(GameData * gameData)
{
	handleInput(gameData);

	auto tempBox = AABoundingBox{m_pos + m_boundingBox.m_min, m_pos + m_boundingBox.m_max};

	glm::vec3 pos = glm::vec3(1.0f, 0.0f, 0.0f);

	if (Collision::hasCollided(tempBox, pos))
	{
		m_model->setColour(glm::vec4(1.0f, 0.0f, 0.f, 1.0f));
	}
	else
	{
		m_model->setColour(glm::vec4(0.0f, 1.0f, 0.f, 1.0f));
	}

	GameObject3D::tick(gameData);
}

void Player::draw(DrawData * drawData)
{
	//Draw model
	m_model->setWorld(this->m_worldMatrix);
	m_model->draw(drawData);
}

void Player::handleInput(GameData* gameData)
{
	if (gameData->m_keyboard.keyIsPressed(Key::KEY_A))
	{
		m_yaw -= 1.0f * gameData->m_deltaTime;
	}

	if (gameData->m_keyboard.keyIsPressed(Key::KEY_D))
	{
		m_yaw += 1.0f * gameData->m_deltaTime;
	}

	if (gameData->m_keyboard.keyIsPressed(Key::KEY_W))
	{
		m_pos += glm::vec3(1.0f, 0.0f, 0.0f) * gameData->m_deltaTime; 
	}

	if (gameData->m_keyboard.keyIsPressed(Key::KEY_S))
	{
		m_pos -= glm::vec3(1.0f, 0.0f, 0.0f) * gameData->m_deltaTime;
	}
}
