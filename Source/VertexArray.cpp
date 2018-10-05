#include "VertexArray.h"

VertexArray::VertexArray()
{
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_rendererID);
}

void VertexArray::init()
{
	glGenVertexArrays(1, &m_rendererID);
}

void VertexArray::addBuffer(const VertexBuffer & vb, const VertexBufferLayout & vbl)
{
	bind();

	vb.bind();
	const auto& elements = vbl.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.m_count, element.m_type, element.m_normalized, 
							  vbl.getStride(), (const void*) offset);

		offset += element.m_count * VertexBufferElement::getSizeOfType(element.m_type);;
	}
}

void VertexArray::bind() const
{
	glBindVertexArray(m_rendererID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}
