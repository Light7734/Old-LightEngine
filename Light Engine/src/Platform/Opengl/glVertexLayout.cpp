#include "ltpch.h"
#include "glVertexLayout.h"
#include "glBuffers.h"

#include <glad/glad.h>

namespace Light {

	glVertexLayout::glVertexLayout(std::shared_ptr<VertexBuffer> buffer, const std::vector<std::pair<std::string, VertexElementType>>& elements)
	{
		LT_PROFILE_FUNC();

		LT_CORE_ASSERT(std::dynamic_pointer_cast<glVertexBuffer>(buffer), "glVertexlayout::glVertexLayout: failed to cast VertexBuffer to glVertexBuffer");

		// vertex elements desc
		std::vector<glVertexElementDesc> vertexElementsDesc;
		vertexElementsDesc.reserve(elements.size());
	
		unsigned int stride = 0;
		for (const auto& element : elements)
		{
			vertexElementsDesc.push_back(GetTypeDesc(element.second, stride));
			stride += vertexElementsDesc.back().typeSize * vertexElementsDesc.back().count;
		}

		// vertex array
		glCreateVertexArrays(1, &m_ArrayID);

		buffer->Bind();
		Bind();

		unsigned int index = 0;
		for (const auto& elementDesc : vertexElementsDesc)
		{
			glVertexAttribPointer(index, elementDesc.count, elementDesc.type, false, stride, (const void*)elementDesc.offset);
			glEnableVertexAttribArray(index++);
		}
	}

	glVertexLayout::~glVertexLayout()
	{
		LT_PROFILE_FUNC();
		glDeleteVertexArrays(1, &m_ArrayID);
	}

	void glVertexLayout::Bind()
	{
		glBindVertexArray(m_ArrayID);
	}

	glVertexElementDesc glVertexLayout::GetTypeDesc(VertexElementType type, unsigned int offset)
	{
		switch (type)
		{
		case VertexElementType::Int:        return { GL_INT         , 1, sizeof(int)   , offset };
		case VertexElementType::Int2:       return { GL_INT         , 2, sizeof(int)   , offset };
		case VertexElementType::Int3:       return { GL_INT         , 3, sizeof(int)   , offset };
		case VertexElementType::Int4:       return { GL_INT         , 4, sizeof(int)   , offset };

		case VertexElementType::UInt:       return { GL_UNSIGNED_INT, 1, sizeof(int)   , offset };
		case VertexElementType::UInt2:      return { GL_UNSIGNED_INT, 2, sizeof(int)   , offset };
		case VertexElementType::UInt3:      return { GL_UNSIGNED_INT, 3, sizeof(int)   , offset };
		case VertexElementType::UInt4:      return { GL_UNSIGNED_INT, 4, sizeof(int)   , offset };

		case VertexElementType::Float:      return { GL_FLOAT       , 1, sizeof(float) , offset };
		case VertexElementType::Float2:     return { GL_FLOAT       , 2, sizeof(float) , offset };
		case VertexElementType::Float3:     return { GL_FLOAT       , 3, sizeof(float) , offset };
		case VertexElementType::Float4:     return { GL_FLOAT       , 4, sizeof(float) , offset };

		case VertexElementType::Double:     return { GL_DOUBLE      , 1, sizeof(double), offset };
		case VertexElementType::Double2:    return { GL_DOUBLE      , 2, sizeof(double), offset };
		case VertexElementType::Double3:    return { GL_DOUBLE      , 3, sizeof(double), offset };
		case VertexElementType::Double4:    return { GL_DOUBLE      , 4, sizeof(double), offset };

		default: LT_CORE_ASSERT(false, "glVertexLayout::GetTypeAttributes: invalid vertex type");
		}
	}

}