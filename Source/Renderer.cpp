#include "Renderer.h"

#include <GL/wglew.h>

#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include "Application.h"

Renderer::~Renderer()
{
	//Delete context once done
	wglDeleteContext(m_deviceContext);
}

const bool Renderer::init(HWND& hWnd)
{
	//Get windows Handle to device context
	m_hdc = GetDC(hWnd);

	//Create a dummy context
	m_deviceContext = wglCreateContext(m_hdc);

	//Make the context current and if failed return false
	if (!wglMakeCurrent(m_hdc, m_deviceContext))
	{
		return false;
	}

	//Initialize modern OpenGL library
	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	//Create attribute list for pixel format
	const int attribList[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0, // End
	};

	int pixelFormat;
	UINT numFormats;

	//If pixel chosen pixel format cannot be chosen, return false
	if (!wglChoosePixelFormatARB(m_hdc, attribList, NULL, 1,
		&pixelFormat, &numFormats))
	{
		return false;
	}

	//Creating context with attributes
	m_deviceContext = wglCreateContextAttribsARB(m_hdc, m_deviceContext, attribList);

#if _DEBUG == 1
	//Print version of OpenGL
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
#endif

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
	VertexBuffer vb = VertexBuffer(positions, 4 * 2 * sizeof(float));

	VertexBufferLayout layout;
	layout.push<float>(2);
	m_va->addBuffer(vb, layout);

	m_ibo = std::make_unique<IndexBuffer>(indices, 6);

	m_shader = std::make_unique<Shader>("Resources/Shaders/VertexShader.glsl", "Resources/Shaders/FragmentShader.frag");

	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//If all initalization suceeded return true
	return true;
}

//Temporary Draw loop
void Renderer::draw(HDC& dc)
{
	glClear(GL_COLOR_BUFFER_BIT);

	
	m_shader->bind();
	m_shader->setUniform4f("u_colour", 0.8f, 0.3f, 0.8f, 1.0f);

	m_va->bind();
	m_ibo->bind();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	SwapBuffers(m_hdc);
}