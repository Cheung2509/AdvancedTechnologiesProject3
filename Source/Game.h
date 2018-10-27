#pragma once

#include "Renderer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "GameData.h"
#include "DrawData.h"
#include "Keyboard.h"
#include "OBBobj.h"

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

	std::vector<std::unique_ptr<OBBobj>> m_gameObjects;
};