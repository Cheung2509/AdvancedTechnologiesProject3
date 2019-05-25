#pragma once

#include <vector>
#include "Vertex.h"
#include <GL/glew.h>

struct VertexBufferElement
{
	unsigned int m_type;
	unsigned int m_count;
	unsigned char m_normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}

		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout() : m_stride(0)
	{
	};
	~VertexBufferLayout() = default;
	
	//Push back
	template<typename T>
	void push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		m_elements.push_back({GL_FLOAT, count, false});
		m_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
		m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
		m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	template<>
	void push<Vertex>(unsigned int count)
	{
		m_elements.push_back({GL_FLOAT, 3, GL_FALSE});
		m_stride += 3 * VertexBufferElement::getSizeOfType(GL_FLOAT);
		m_elements.push_back({GL_FLOAT, 2, GL_FALSE});
		m_stride += 2 * VertexBufferElement::getSizeOfType(GL_FLOAT);
		m_elements.push_back({GL_FLOAT, 4, GL_FALSE});
		m_stride += 4 * VertexBufferElement::getSizeOfType(GL_FLOAT);
		m_elements.push_back({GL_FLOAT, 4, GL_FALSE});
		m_stride += 4 * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}


	inline const std::vector<VertexBufferElement> getElements() const { return m_elements; }
	inline unsigned int getStride() const { return m_stride; }

private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
};