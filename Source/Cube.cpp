#include "Cube.h"

#include "DrawData.h"

#include "glm/gtc/matrix_transform.hpp"

Cube::Cube()
{
	float positions[] = 
	{
		// front
		-1.0, -1.0,  1.0,
		 1.0, -1.0,  1.0,
		 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
	};

	unsigned int indices[] =
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3,
	};

	m_va = std::make_unique<VertexArray>();
	m_va->init();

	m_vb = std::make_unique<VertexBuffer>(positions, (4 * 3) * 3  *sizeof(float));
	VertexBufferLayout layout;
	layout.push<float>(3);
	m_va->addBuffer(*m_vb, layout);

	m_ib = std::make_unique<IndexBuffer>(indices, sizeof(indices));

	m_shader = std::make_unique<Shader>("Resources/Shaders/VertexShader.glsl", "Resources/Shaders/FragmentShader.frag");
}

Cube::~Cube()
{
}

void Cube::init()
{
}

void Cube::tick(GameData * gameData)
{
	GameObject3D::tick(gameData);
}

void Cube::draw(DrawData* drawData)
{
	glm::mat4 mvp = drawData->m_camera->getProjection() * drawData->m_camera->getView() *
		m_worldMatrix;

	m_shader->setUniform4f("u_colour", 1.0f, 0.0f, 0.0f, 1.0f);
	m_shader->setUniform4fv("u_MVP", 1, GL_FALSE, mvp);
	
	drawData->m_renderer->draw(*m_va, *m_ib, *m_shader);
}
