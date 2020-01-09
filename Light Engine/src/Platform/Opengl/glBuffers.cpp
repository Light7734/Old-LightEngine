#include "ltpch.h"
#include "glBuffers.h"

#include <glad/glad.h>

namespace Light {

	glVertexBuffer::glVertexBuffer(float* vertices, unsigned int size)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, size, vertices, GL_DYNAMIC_DRAW);
	}

	glVertexBuffer::~glVertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void glVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void* glVertexBuffer::Map()
	{
		return glMapNamedBuffer(m_BufferID, GL_WRITE_ONLY);
	}

	void glVertexBuffer::UnMap()
	{
		glUnmapNamedBuffer(m_BufferID);
	}

	glIndexBuffer::glIndexBuffer(unsigned int* indices, unsigned int size)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, size, indices, GL_STATIC_DRAW);
	}

	glIndexBuffer::~glIndexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void glIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

}