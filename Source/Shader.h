#pragma once

#include <string>
#include <unordered_map>

class Shader
{
public:
	Shader() = default;
	Shader(const std::string& vs, const std::string& fs);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	int getUniformLocation(const std::string& name);
	
	unsigned int compileShader(unsigned int type, const std::string& source);
	const std::string parseShader(const std::string& filepath);
	unsigned int createShader(const std::string & vertexShader, const std::string & fragmentShader);

private:
	unsigned int m_rendererID;
	std::unordered_map<std::string, int> m_uniformLocationCache;
};