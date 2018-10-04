#pragma once

#define GL_GLEXT_PROTOTYPES

#include <GL/glew.h>
#include <Windows.h>
#include <memory>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "VertexBuffer.h"

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
	HDC m_hdc;

	//Temporary stuff
	std::unique_ptr<VertexBuffer> m_vb;
};