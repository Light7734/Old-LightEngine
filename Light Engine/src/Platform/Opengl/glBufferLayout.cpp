#include "ltpch.h"
#include "glBufferLayout.h"

#include "glBuffers.h"
#include "glVertexLayout.h"

#include <glad/glad.h>

namespace Light {

	glBufferLayout::glBufferLayout(std::shared_ptr<VertexBuffer> buffer, std::shared_ptr<VertexLayout> layout)
	{
		std::shared_ptr<glVertexLayout> glpLayout = std::dynamic_pointer_cast<glVertexLayout>(layout);
		LT_CORE_ASSERT(glpLayout, "glBufferLayout::glBufferLayout: Failed to cast VertexBuffer to glVertexBuffer");

		glCreateVertexArrays(1, &m_ArrayID);

		buffer->Bind();
		Bind();

		unsigned int index = 0;
		for (glVertexAttributes attribute : glpLayout->GetAttribtues())
		{
			if (attribute.type <= GL_UNSIGNED_INT)
				glVertexAttribIPointer(index, attribute.count, attribute.type, glpLayout->GetStride(), (const void*)attribute.offset);
			else
				glVertexAttribPointer(index, attribute.count, attribute.type, false, glpLayout->GetStride(), (const void*)attribute.offset);
			glEnableVertexAttribArray(index++);
		}
	}

	glBufferLayout::~glBufferLayout()
	{
		glDeleteVertexArrays(1, &m_ArrayID);
	}


	void glBufferLayout::Bind()
	{
		glBindVertexArray(m_ArrayID);
	}

}