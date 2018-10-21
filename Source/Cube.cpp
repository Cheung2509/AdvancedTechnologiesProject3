#include "Cube.h"

#include "DrawData.h"
#include "GameData.h"
#include "Key.h"

#include "glm/gtc/matrix_transform.hpp"

Cube::Cube()
{
	float positions[] =
	{
		// front			//Colour
		-1.0, -1.0,  1.0,	1.0f, 0.0f, 0.0f, 1.0f,
		 1.0, -1.0,  1.0,	0.0f, 1.0f, 0.0f, 1.0f,
		 1.0,  1.0,  1.0,	0.0f, 0.0f, 1.0f, 1.0f,
		-1.0,  1.0,  1.0,	0.5f, 0.5f, 0.5f, 1.0f,
		// back
		-1.0, -1.0, -1.0,	1.0f, 0.0f, 0.0f, 1.0f,
		 1.0, -1.0, -1.0,	0.0f, 1.0f, 0.0f, 1.0f,
		 1.0,  1.0, -1.0,	0.0f, 0.0f, 1.0f, 1.0f,
		-1.0,  1.0, -1.0,	0.5f, 0.5f, 0.5f, 1.0f
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

	m_vb = std::make_unique<VertexBuffer>(positions, sizeof(positions));
	VertexBufferLayout layout;
	layout.push<float>(3);
	layout.push<float>(4);
	m_va->addBuffer(*m_vb, layout);
	
	
	m_ib = std::make_unique<IndexBuffer>(indices, sizeof(indices));

	m_shader = std::make_unique<Shader>("Resources/Shaders/VertexShader.glsl", "Resources/Shaders/FragmentShader.frag");
}

Cube::~Cube()
{
}

void Cube::init()
{
	float positions[] =
	{
		// front			//Colour
		-1.0, -1.0,  1.0,	1.0f, 0.0f, 0.0f, 1.0f,
		 1.0, -1.0,  1.0,	0.0f, 1.0f, 0.0f, 1.0f,
		 1.0,  1.0,  1.0,	0.0f, 0.0f, 1.0f, 1.0f,
		-1.0,  1.0,  1.0,	0.5f, 0.5f, 0.5f, 1.0f,
		// back
		-1.0, -1.0, -1.0,	1.0f, 0.0f, 0.0f, 1.0f,
		 1.0, -1.0, -1.0,	0.0f, 1.0f, 0.0f, 1.0f,
		 1.0,  1.0, -1.0,	0.0f, 0.0f, 1.0f, 1.0f,
		-1.0,  1.0, -1.0,	0.5f, 0.5f, 0.5f, 1.0f
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

	m_vb = std::make_unique<VertexBuffer>(positions, sizeof(positions));
	VertexBufferLayout layout;
	layout.push<float>(3);
	layout.push<float>(4);
	m_va->addBuffer(*m_vb, layout);


	m_ib = std::make_unique<IndexBuffer>(indices, sizeof(indices));

	m_shader = std::make_shared<Shader>("Resources/Shaders/VertexShader.glsl", "Resources/Shaders/FragmentShader.frag");
}

void Cube::init(std::string vertexShader, std::string fragmentShader)
{
	float positions[] =
	{
		// front			//Colour
		-1.0, -1.0,  1.0,	1.0f, 0.0f, 0.0f, 1.0f,
		 1.0, -1.0,  1.0,	0.0f, 1.0f, 0.0f, 1.0f,
		 1.0,  1.0,  1.0,	0.0f, 0.0f, 1.0f, 1.0f,
		-1.0,  1.0,  1.0,	0.5f, 0.5f, 0.5f, 1.0f,
		// back
		-1.0, -1.0, -1.0,	1.0f, 0.0f, 0.0f, 1.0f,
		 1.0, -1.0, -1.0,	0.0f, 1.0f, 0.0f, 1.0f,
		 1.0,  1.0, -1.0,	0.0f, 0.0f, 1.0f, 1.0f,
		-1.0,  1.0, -1.0,	0.5f, 0.5f, 0.5f, 1.0f
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

	m_vb = std::make_unique<VertexBuffer>(positions, sizeof(positions));
	VertexBufferLayout layout;
	layout.push<float>(3);
	layout.push<float>(4);
	m_va->addBuffer(*m_vb, layout);


	m_ib = std::make_unique<IndexBuffer>(indices, sizeof(indices));

	m_shader = std::make_shared<Shader>(vertexShader, fragmentShader);
}

void Cube::init(std::shared_ptr<Shader> shader)
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
		-1.0,  1.0, -1.0
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

	m_vb = std::make_unique<VertexBuffer>(positions, sizeof(positions));
	VertexBufferLayout layout;
	layout.push<float>(3);
	m_va->addBuffer(*m_vb, layout);


	m_ib = std::make_unique<IndexBuffer>(indices, sizeof(indices));

	m_shader = shader;

	//Just preset stuff for now
	for (int i = 0; i < sizeof(positions) / sizeof(positions[0]); i++)
	{
		m_vertices.emplace_back(positions[i], positions[i++], positions[i++]);
	}

	calculateminMax();

	m_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
}

void Cube::tick(GameData * gameData)
{
	VBO::tick(gameData);
}

void Cube::draw(DrawData* drawData)
{
	VBO::draw(drawData);
}