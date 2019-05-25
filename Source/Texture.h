#pragma once

#include <string>

#include "assimp/types.h"

class Texture
{
public:
	Texture(const char* path, const std::string& directory, const std::string& type);
	~Texture();

	void bind(const int& slot = 0) const;
	void unbind() const;

	inline const std::string& getPath() const { return m_path; }
	inline const std::string& getType() const { return m_type; }
protected:
	unsigned int m_id;
	std::string m_type;
	std::string m_path;

	int m_width;
	int m_height;
	int m_BPP;

	unsigned char* m_localBuffer;
};