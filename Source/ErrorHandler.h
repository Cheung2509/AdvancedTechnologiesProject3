#pragma once

#include <iostream>

#include "GL/glew.h"
#include "spdlog/spdlog.h"

#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		auto message = "[OpenGL Error] (" + error + ')' + std::string(function) + " " + file + ":" + std::to_string(line);
		spdlog::error(message);
		return false;
	}

	return true;
}