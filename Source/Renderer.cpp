#include "Renderer.h"

#include <GL/wglew.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"

static unsigned int compileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;

		std::cout << message << std::endl;

		glDeleteShader(id);

		return 0;
	}

	return id;
}

static unsigned int createShader(const std::string & vertexShader, const std::string & fragmentShader)
{

	unsigned int program = glCreateProgram();

	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}



static const std::string parseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	std::string line;
	std::stringstream ss;

	while (std::getline(stream, line))
	{
		ss << line << '\n';
	}

	return ss.str();
}

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

	unsigned int source = createShader(parseShader("Resources/VertexShader.glsl"), parseShader("Resources/FragmentShader.frag"));
	glUseProgram(source);

	//Create temporary vertex buffer
	float positions[6] =
	{
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	m_vb = std::make_unique<VertexBuffer>(positions, 6 * 2 * sizeof(float));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	

	//If all initalization suceeded return true
	return true;
}

//Temporary Draw loop
void Renderer::draw(HDC& dc)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	SwapBuffers(m_hdc);
}


