#include "Renderer.h"

#include <GL/wglew.h>

#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"

#include <iostream>

#include "Application.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ErrorHandler.h"

Renderer::~Renderer()
{
	//Delete context once done
	wglDeleteContext(m_deviceContext);
}

const bool Renderer::init(HWND& hWnd)
{
	//Get windows Handle to device context
	HDC hdc = GetDC(hWnd);

	//Create a dummy context
	m_deviceContext = wglCreateContext(hdc);

	//Make the context current and if failed return false
	if (!wglMakeCurrent(hdc, m_deviceContext))
	{
		return false;
	}

	//Initialize modern OpenGL library
	if (glewInit() != GLEW_OK)
	{
		return false;
	}

#if _DEBUG == 1
	//Print version of OpenGL
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
#endif

	//Enable OpenGL options 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//If all initalization suceeded return true
	return true;
}

void Renderer::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::draw(const VertexArray & va, const IndexBuffer& ib, const Shader & shader)
{
	shader.bind();
	va.bind();
	ib.bind();
	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}
