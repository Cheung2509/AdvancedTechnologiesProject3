
#include "Shader.h"

#include "GL/glew.h"
#include "ErrorHandler.h"

#include "spdlog/spdlog.h"

#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string & vs, const std::string& fs) : m_rendererID(0)
{
	m_rendererID = createShader(vs, fs);
}

Shader::~Shader()
{
	glDeleteProgram(m_rendererID);
}

void Shader::bind() const
{
	glUseProgram(m_rendererID);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::setBool(const std::string & name, bool v0)
{
	GLCALL(glUniform1i(getUniformLocation(name), v0));
}

void Shader::setUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	glUniform4f(getUniformLocation(name) , v0, v1, v2, v3);
}

void Shader::setUniform4fv(const std::string & name, int count , bool transpose, glm::mat4& matrix)
{
	GLCALL(glUniformMatrix4fv(getUniformLocation(name), count, transpose, glm::value_ptr(matrix)));
}



int Shader::getUniformLocation(const std::string & name)
{
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[name];

	GLCALL(int location = glGetUniformLocation(m_rendererID, name.c_str()));

	m_uniformLocationCache[name] = location;

	if(location == -1)
	{
		spdlog::warn("no Location");
	}

	return location;
}

unsigned int Shader::createShader(const std::string & vertexShader, const std::string & fragmentShader)
{

	unsigned int program = glCreateProgram();

	//Compile 
	unsigned int vs = compileShader(GL_VERTEX_SHADER, parseShader(vertexShader));
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, parseShader(fragmentShader));

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

//Function used to read shader files
const std::string Shader::parseShader(const std::string& filepath)
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

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
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

		std::string nType = (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		spdlog::error("Failed to compile " + nType);

		spdlog::error(message);

		glDeleteShader(id);

		
		return 0;
	}

	return id;
}
