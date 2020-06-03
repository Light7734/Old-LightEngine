#include "ltpch.h"
#include "glBuffers.h"

#include <glad/glad.h>

namespace Light {

	// ConstantBuffer //
	glConstantBuffer::glConstantBuffer(ConstantBufferIndex index, unsigned int size)
		: m_Index(index)
	{
		LT_PROFILE_FUNC();

		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, size, NULL, GL_DYNAMIC_DRAW);

		Bind();
	}

	glConstantBuffer::~glConstantBuffer()
	{
		LT_PROFILE_FUNC();
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
		LT_PROFILE_FUNC();

		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, size, vertices, GL_DYNAMIC_DRAW);
	}

	glVertexBuffer::~glVertexBuffer()
	{
		LT_PROFILE_FUNC();
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
	glIndexBuffer::glIndexBuffer(unsigned int* indices, unsigned int count)
	{
		LT_PROFILE_FUNC();

		bool noIndices = false;
		if (!indices)
		{
			LT_CORE_ASSERT(count % 6 == 0, "glIndexBuffer::glIndexBuffer: indices can only be null if count is multiple of 6");

			noIndices = true;
			indices = new unsigned int[count];

			unsigned int offset = 0;
			for (int i = 0; i < count; i += 6)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}
		}

		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		if (noIndices)
		{
			delete[] indices;
			indices = nullptr;
		}
	}

	glIndexBuffer::~glIndexBuffer()
	{
		LT_PROFILE_FUNC();
		glDeleteBuffers(1, &m_BufferID);
	}

	void glIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

}