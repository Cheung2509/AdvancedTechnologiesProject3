#include "Game.h"

#include "ErrorHandler.h"
#include "Cube.h"
#include "Player.h"
#include "CollidableCube.h"

bool Game::init()
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>
		("Resources/Shaders/VertexShader.glsl", "Resources/Shaders/FragmentShader.frag");


	auto m_cube = std::make_unique<Cube>();
	m_cube->init(shader);
	m_cube->setScale(glm::vec3(0.1f, 0.1f, 0.1f));

	auto player = std::make_unique<Player>();
	player->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	player->init(std::move(m_cube));
	m_gameObjects.push_back(std::move(player));

	auto cube2 = std::make_unique<Cube>();
	cube2->init(shader);
	cube2->setScale(glm::vec3(0.1f, 0.1f, 0.1f));

	auto CollideCube = std::make_unique<CollidableCube>();
	CollideCube->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	CollideCube->init(std::move(cube2));
	CollideCube->setPos(glm::vec3(2.0f, 0.0f, 0.0f));
	m_gameObjects.push_back(std::move(CollideCube));

	auto cube3 = std::make_unique<Cube>();
	cube3->init(shader);
	cube3->setScale(glm::vec3(0.1f, 0.1f, 0.1f));

	auto CollideCube2 = std::make_unique<CollidableCube>();
	CollideCube2->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	CollideCube2->init(std::move(cube3));
	CollideCube2->setPos(glm::vec3(-2.0f, 0.0f, 0.0f));
	m_gameObjects.push_back(std::move(CollideCube2));

	auto cube4 = std::make_unique<Cube>();
	cube4->init(shader);
	cube4->setScale(glm::vec3(0.1f, 0.1f, 0.1f));

	auto CollideCube3 = std::make_unique<CollidableCube>();
	CollideCube3->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	CollideCube3->init(std::move(cube4));
	CollideCube3->setPos(glm::vec3(0.0f, 0.0f, 2.0f));
	m_gameObjects.push_back(std::move(CollideCube3));

	auto cube5 = std::make_unique<Cube>();
	cube5->init(shader);
	cube5->setScale(glm::vec3(0.1f, 0.1f, 0.1f));

	auto CollideCube4 = std::make_unique<CollidableCube>();
	CollideCube4->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	CollideCube4->init(std::move(cube5));
	CollideCube4->setPos(glm::vec3(0.0f, 0.0f, -2.0f));
	m_gameObjects.push_back(std::move(CollideCube4));


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

	if (keyboard.keyIsPressed(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	m_drawData->m_camera->tick(m_gameData.get());

	for (auto& object : m_gameObjects)
	{
		object->tick(m_gameData.get());
	}

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		for (int j = 0; i < m_gameObjects.size(); i++)
		{
			if (AABBobj::checkCollision(*m_gameObjects[i], *m_gameObjects[j]))
			{
				m_gameObjects[i]->setColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				m_gameObjects[j]->setColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				break;
			}
			else
			{
				m_gameObjects[i]->setColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
				m_gameObjects[j]->setColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			}
		}
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
