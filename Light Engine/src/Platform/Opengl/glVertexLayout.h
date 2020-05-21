#pragma once

#include "Core/Core.h"

#include "Renderer/VertexLayout.h"

namespace Light {

	struct glVertexElementDesc
	{
		unsigned int type;
		unsigned int count;
		unsigned int typeSize;
		unsigned int offset;
	};

	class glVertexLayout : public VertexLayout
	{
	private:
		unsigned int m_ArrayID;
	public:
		glVertexLayout(std::shared_ptr<VertexBuffer> buffer, const std::vector<std::pair<std::string, VertexElementType>>& elements);
		~glVertexLayout();

		void Bind() override;
	private:
		glVertexElementDesc glVertexLayout::GetTypeDesc(VertexElementType type, unsigned int offset);
	};

}