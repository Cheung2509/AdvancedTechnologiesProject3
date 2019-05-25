#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader
{
public:
	Shader() = default;
	Shader(const std::string& vs, const std::string& fs);
	~Shader();

	void bind() const;
	void unbind() const;

	void setBool(const std::string& name, bool v0);
	void setUniform1i(const std::string& name, int v0);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniform4fv(const std::string& name, int count, bool transpose, glm::mat4& matrix);
	int getUniformLocation(const std::string& name);

	inline unsigned int getID() const { return m_rendererID; }
private:
	unsigned int compileShader(unsigned int type, const std::string& source);
	const std::string parseShader(const std::string& filepath);
	unsigned int createShader(const std::string & vertexShader, const std::string & fragmentShader);

private:
	unsigned int m_rendererID;
	std::unordered_map<std::string, int> m_uniformLocationCache;
};