#include "Renderer.h"

#include <GL/wglew.h>

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

	//Create attribute list for p
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

	//If all initalization suceeded return true
	return true;
}

//Temporary Draw loop
void Renderer::draw(HDC& dc)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.0f, 0.5f);
	glVertex2f(0.5f, -0.5f);
	glEnd();

	SwapBuffers(m_hdc);
}
