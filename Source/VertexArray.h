#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void init();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl);

	void bind() const;
	void unbind() const;
private:
	unsigned int m_rendererID;
};