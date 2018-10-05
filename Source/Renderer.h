#pragma once

#define GL_GLEXT_PROTOTYPES

#include <GL/glew.h>
#include <Windows.h>
#include <memory>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

class VertexArray;
class IndexBuffer;
class Shader;

class Renderer
{
public:
	Renderer() = default;
	~Renderer();

	const bool init(HWND& hWnd);

	void clear() const;
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);

	const HGLRC& getContext() const { return m_deviceContext; }
private:
	HGLRC m_deviceContext;
};