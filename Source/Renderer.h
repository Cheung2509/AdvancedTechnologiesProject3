#pragma once

#define GL_GLEXT_PROTOTYPES

#include <Windows.h>
#include <gl/GL.h>
#include <memory>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

class Renderer
{
public:
	Renderer() = default;
	~Renderer();

	const bool init(HWND& hWnd);
	
	void draw(HDC& dc);

	const HGLRC& getContext() const { return m_deviceContext; }
private:
	HGLRC m_deviceContext;
};