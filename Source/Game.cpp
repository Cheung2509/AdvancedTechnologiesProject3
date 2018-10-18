#include "Game.h"

#include "ErrorHandler.h"
#include "Cube.h"

bool Game::init()
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>
		("Resources/Shaders/VertexShader.glsl", "Resources/Shaders/FragmentShader.frag");

	for (int x = 0; x < 10; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			auto m_cube = std::make_unique<Cube>();
			m_cube->init(shader);
			m_cube->setPos(glm::vec3(x, 0.0f, z));
			m_cube->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
			m_gameObjects.push_back(std::move(m_cube));
		}
	}

	m_gameData = std::make_unique<GameData>();

	m_drawData = std::make_unique<DrawData>();
	m_drawData->m_camera = std::make_shared<Camera>(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);

	m_drawData->m_camera->setPos(glm::vec3(4.0f, 3.0f, 3.0f));

	return true;
}

void Game::tick(const float& deltaTime, const Keyboard& keyboard, const Mouse& mouse)
{
	m_gameData->m_deltaTime = deltaTime;
	m_gameData->m_keyboard = keyboard;
	m_gameData->m_mouse = mouse;

	m_drawData->m_camera->tick(m_gameData.get());

	for (auto& object : m_gameObjects)
	{
		object->tick(m_gameData.get());
	}
}


void Game::draw(std::shared_ptr<Renderer> renderer)
{
	renderer->clear();

	m_drawData->m_renderer = renderer;

	for (auto& object : m_gameObjects)
	{
		object->draw(m_drawData.get());
	}
}
