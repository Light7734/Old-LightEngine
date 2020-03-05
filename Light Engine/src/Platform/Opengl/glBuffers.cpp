#include "ltpch.h"
#include "glBuffers.h"

#include <glad/glad.h>

namespace Light {

	// UniformBuffers //
	glUniformBuffers::glUniformBuffers()
	{
		glCreateBuffers(1, &m_ViewProjBuffer);
		glNamedBufferData(m_ViewProjBuffer, sizeof(glm::mat4) * 2, NULL, GL_DYNAMIC_DRAW);

		glBindBufferBase(GL_UNIFORM_BUFFER, UBufferIndex_ViewProjection, m_ViewProjBuffer);
	}

	glUniformBuffers::~glUniformBuffers()
	{
		glDeleteBuffers(1, &m_ViewProjBuffer);
	}

	void glUniformBuffers::SetViewProjMatrixImpl(const glm::f32* view, const glm::f32* proj)
	{
		glm::mat4* map = (glm::mat4*)glMapNamedBuffer(m_ViewProjBuffer, GL_WRITE_ONLY);

		memcpy(map, view, sizeof(glm::mat4));
		memcpy(map + 1, proj, sizeof(glm::mat4));

		glUnmapNamedBuffer(m_ViewProjBuffer);
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