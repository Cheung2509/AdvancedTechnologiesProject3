#include "Game.h"

#include "ErrorHandler.h"

bool Game::init()
{
	m_cube = std::make_unique<Cube>();
	m_cube->init();
	m_cube->setPos(glm::vec3(0.0f, 0.0f, 0.0f));

	m_gameData = std::make_unique<GameData>();

	m_drawData = std::make_unique<DrawData>();
	m_drawData->m_camera = std::make_shared<Camera>(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);

	m_drawData->m_camera->setPos(glm::vec3(4.0f, 3.0f, 3.0f));

	return true;
}

void Game::tick(const float& deltaTime)
{
	m_gameData->m_deltaTime = deltaTime;

	m_drawData->m_camera->tick(m_gameData.get());

	m_cube->tick(m_gameData.get());
}


void Game::draw(std::shared_ptr<Renderer> renderer)
{
	renderer->clear();

	m_cube->draw(m_drawData.get());
}
