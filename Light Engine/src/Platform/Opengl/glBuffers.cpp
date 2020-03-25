#include "ltpch.h"
#include "glBuffers.h"

#include "Renderer/GraphicsContext.h"

#include <glad/glad.h>

namespace Light {

	// ConstantBuffer //
	glConstantBuffer::glConstantBuffer(ConstantBufferIndex index, unsigned int size)
		: m_Index(index)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, size, NULL, GL_DYNAMIC_DRAW);

		Bind();
	}

	glConstantBuffer::~glConstantBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void glConstantBuffer::Bind()
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_Index, m_BufferID);
	}

	void* glConstantBuffer::Map()
	{
		return glMapNamedBuffer(m_BufferID, GL_WRITE_ONLY);
	}

	void glConstantBuffer::UnMap()
	{
		glUnmapNamedBuffer(m_BufferID);
	}

	// VertexBuffer //
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

	// IndexBuffer //
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