#include "Renderer.h"

#include <GL/wglew.h>

#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"

#include <iostream>

#include "Application.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

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
	if (!wglChoosePixelFormatARB(hdc, attribList, NULL, 1,
		&pixelFormat, &numFormats))
	{
		return false;
	}

	//Creating context with attributes
	m_deviceContext = wglCreateContextAttribsARB(hdc, m_deviceContext, attribList);

#if _DEBUG == 1
	//Print version of OpenGL
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
#endif

	//If all initalization suceeded return true
	return true;
}



void Renderer::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::draw(const VertexArray & va, const IndexBuffer& ib, const Shader & shader, HDC hdc)
{
	shader.bind();
	va.bind();
	ib.bind();
	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);

	//SwapBuffers(hdc);
}
