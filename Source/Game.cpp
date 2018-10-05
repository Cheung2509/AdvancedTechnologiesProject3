#include "Game.h"

#include "ErrorHandler.h"

bool Game::init()
{

	//Create temporary vertex buffer
	float positions[] =
	{
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	unsigned int indices[] =
	{
		0, 1 ,2,
		2, 3, 0
	};

	unsigned int m_vao;
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	m_va = std::make_unique<VertexArray>();
	m_va->init();

	m_vb = std::make_unique<VertexBuffer>(positions, 4 * 2 * sizeof(float));
	VertexBufferLayout layout;
	layout.push<float>(2);
	m_va->addBuffer(*m_vb, layout);

	m_ibo = std::make_unique<IndexBuffer>(indices, 6);

	m_shader = std::make_unique<Shader>("Resources/Shaders/VertexShader.glsl", "Resources/Shaders/FragmentShader.frag");

	return true;
}

void Game::tick()
{
}

void Game::draw(std::shared_ptr<Renderer> renderer)
{
	m_shader->setUniform4f("u_colour", r, 0.3f, 0.8f, 1.0f);

	renderer->clear();

	renderer->draw(*m_va, *m_ibo, *m_shader);
}
