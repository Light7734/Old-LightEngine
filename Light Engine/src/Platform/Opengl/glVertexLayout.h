#pragma once

#include "Core/Core.h"

#include "Renderer/VertexLayout.h"

namespace Light {

	struct glVertexAttributes
	{
		unsigned int type;
		unsigned int count;
		unsigned int size;
		unsigned int offset;
	};

	class glVertexLayout : public VertexLayout
	{
	private:
		std::vector<glVertexAttributes> m_Attributes;
		unsigned int m_Stride = 0;
	public:
		glVertexLayout(std::initializer_list<std::pair<const char*, VertexType>> initList);

		inline std::vector<glVertexAttributes> GetAttribtues() { return m_Attributes; }

		inline unsigned int GetStride() { return m_Stride; }
	private:
		inline void UpdateStride() { m_Stride += m_Attributes.back().count * m_Attributes.back().size; }
		glVertexAttributes GetTypeAttributes(VertexType type);
	};

}