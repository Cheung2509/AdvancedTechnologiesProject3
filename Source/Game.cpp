#include "Game.h"

#include "ErrorHandler.h"
#include "Model.h"
#include "Helper.h"

bool Game::init()
{
	//Initialise game objects
	std::unique_ptr<Model> model = std::make_unique<Model>("Resources/Models/vampire/dancing_vampire.dae");
	//model->rotate(180.0f, glm::vec3(0.0f, 0.0f, 0.0));
	m_gameObjects.emplace_back(std::move(model));

	//Initialise data structures to pass though
	m_gameData = std::make_unique<GameData>();

	m_drawData = std::make_unique<DrawData>();
	m_drawData->m_camera = std::make_shared<Camera>(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 1000.0f,
													glm::vec3(0.0f, 0.1f, 0.0f), glm::vec3(0.0f, 60.0f, 0.0f));

	m_drawData->m_camera->setPos(glm::vec3(0.0f, -125.0f, 150.0f));

	return true;
}

void Game::tick(const float& deltaTime, const Keyboard& keyboard, const Mouse& mouse)
{
	m_gameData->m_deltaTime = deltaTime;
	m_gameData->m_runTime += deltaTime;
	m_gameData->m_prevKeyboard = m_gameData->m_keyboard;
	m_gameData->m_keyboard = keyboard;
	m_gameData->m_mouse = mouse;

	if (keyboard.keyIsPressed(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	m_drawData->m_camera->tick(m_gameData.get());

	for (auto& object : m_gameObjects)
	{
		object->tick(m_gameData.get());
	}
}


void Game::draw(std::shared_ptr<Renderer> renderer)
{
	renderer->clear();

	for (auto& object : m_gameObjects)
	{
		object->draw(m_drawData.get());
	}
}
