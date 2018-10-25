#pragma once

#include "Renderer.h"
#include "GameData.h"
#include "DrawData.h"
#include "Keyboard.h"

#include "ECS.h"

class Game
{
public:
	Game() = default;
	~Game() = default;

	bool init();

	void tick(const float& deltaTime, const Keyboard& keyboard, const Mouse& );
	void draw(std::shared_ptr<Renderer> renderer);
private:
	std::unique_ptr<DrawData> m_drawData;
	std::unique_ptr<GameData> m_gameData;

	Manager m_EntityManager;
};