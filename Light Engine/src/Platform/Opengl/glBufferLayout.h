#pragma once

#include "Core/Core.h"

#include "Renderer/BufferLayout.h"

namespace Light {

	class VertexBuffer;
	class VertexLayout;

	class glBufferLayout : public BufferLayout
	{
	private:
		unsigned int m_ArrayID;
	public:
		glBufferLayout(std::shared_ptr<VertexBuffer> buffer, std::shared_ptr<VertexLayout> layout);
		~glBufferLayout();

		void Bind() override;
	};

}