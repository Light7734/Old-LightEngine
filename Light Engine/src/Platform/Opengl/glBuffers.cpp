#include "ltpch.h"
#include "glBuffers.h"

#include <glad/glad.h>

namespace Light {

	glVertexBuffer::glVertexBuffer(float* vertices, unsigned int size)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, size, vertices, GL_STATIC_DRAW);
	}

	glVertexBuffer::~glVertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void glVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

}