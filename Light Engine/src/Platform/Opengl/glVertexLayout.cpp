#include "ltpch.h"
#include "glVertexLayout.h"

#include <glad/glad.h>

namespace Light {

	glVertexLayout::glVertexLayout(std::initializer_list<std::pair<const char*, VertexType>> initList)
	{
		for (std::pair<const char*, VertexType> listElement : initList)
		{
			m_Attributes.emplace_back(GetTypeAttributes(listElement.second));
			UpdateStride();
		}
	}

	glVertexAttributes glVertexLayout::GetTypeAttributes(VertexType type)
	{
		switch (type)
		{
		case Light::VertexType::Int:        return { GL_INT         , 1, sizeof(int)   , m_Stride };
		case Light::VertexType::Int2:       return { GL_INT         , 2, sizeof(int)   , m_Stride };
		case Light::VertexType::Int3:       return { GL_INT         , 3, sizeof(int)   , m_Stride };
		case Light::VertexType::Int4:       return { GL_INT         , 4, sizeof(int)   , m_Stride };

		case Light::VertexType::UInt:       return { GL_UNSIGNED_INT, 1, sizeof(int)   , m_Stride };
		case Light::VertexType::UInt2:      return { GL_UNSIGNED_INT, 2, sizeof(int)   , m_Stride };
		case Light::VertexType::UInt3:      return { GL_UNSIGNED_INT, 3, sizeof(int)   , m_Stride };
		case Light::VertexType::UInt4:      return { GL_UNSIGNED_INT, 4, sizeof(int)   , m_Stride };

		case Light::VertexType::Float:      return { GL_FLOAT       , 1, sizeof(float) , m_Stride };
		case Light::VertexType::Float2:     return { GL_FLOAT       , 2, sizeof(float) , m_Stride };
		case Light::VertexType::Float3:     return { GL_FLOAT       , 3, sizeof(float) , m_Stride };
		case Light::VertexType::Float4:     return { GL_FLOAT       , 4, sizeof(float) , m_Stride };

		case Light::VertexType::Double:     return { GL_DOUBLE      , 1, sizeof(double), m_Stride };
		case Light::VertexType::Double2:    return { GL_DOUBLE      , 2, sizeof(double), m_Stride };
		case Light::VertexType::Double3:    return { GL_DOUBLE      , 3, sizeof(double), m_Stride };
		case Light::VertexType::Double4:    return { GL_DOUBLE      , 4, sizeof(double), m_Stride };

		default: LT_CORE_ASSERT(false, "glVertexLayout::GetTypeAttributes: Invalid vertex type");
		}
	}

}