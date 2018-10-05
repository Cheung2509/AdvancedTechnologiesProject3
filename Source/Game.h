#pragma once

#include "Renderer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"

class Game
{
public:
	Game() = default;
	~Game() = default;

	bool init();

	void tick();
	void draw(std::shared_ptr<Renderer> renderer);
private:

	std::unique_ptr<VertexArray> m_va;
	std::unique_ptr<VertexBuffer> m_vb;
	std::unique_ptr<IndexBuffer> m_ibo;
	std::unique_ptr<Shader> m_shader;

	float r = 0;
	float increment = 0.05;
};