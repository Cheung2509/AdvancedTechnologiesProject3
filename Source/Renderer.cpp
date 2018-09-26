#include "Renderer.h"

#include <gl/GLU.h>

Renderer::~Renderer()
{
	//Delete context once done
	wglDeleteContext(m_deviceContext);
}

const bool Renderer::init(HWND& hWnd)
{
	//Get windows Handle to device context
	HDC windowDCHandler = GetDC(hWnd);
	
	//Create and assign device context
	m_deviceContext = wglCreateContext(windowDCHandler);

	//Make the context current and if failed return false
	if (!wglMakeCurrent(windowDCHandler, m_deviceContext))
	{
		return false;
	}

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

	SwapBuffers(dc);
}
