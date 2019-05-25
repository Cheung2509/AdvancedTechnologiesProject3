#include "Texture.h"

#include "Vendor/SOIL2/SOIL2.h"

#include "ErrorHandler.h"

Texture::Texture(const char * path, const std::string & directory, const std::string& type)
	: m_id(0), m_type(type), m_path(path), m_localBuffer(nullptr)
{
	std::string fileName = std::string(path);
	fileName = directory + '/' + fileName;

	GLCALL(glGenTextures(1, &m_id));

	m_localBuffer = SOIL_load_image(fileName.c_str(), &m_width, &m_height, 0, SOIL_LOAD_RGB);

	GLCALL(glBindTexture(GL_TEXTURE_2D, m_id));
	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_localBuffer));
	GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	
	
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

	if(m_localBuffer)
	{
		SOIL_free_image_data(m_localBuffer);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

void Texture::bind(const int& slot) const
{
	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_id));
}

void Texture::unbind() const
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));;
}
