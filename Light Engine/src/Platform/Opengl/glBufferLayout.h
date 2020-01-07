#pragma once

#include "Renderer/BufferLayout.h"

#include "Core/Core.h"

namespace Light {

	class VertexBuffer;

	class glBufferLayout : public BufferLayout
	{
	private:
		unsigned int m_ArrayID = 0;
	public:
		glBufferLayout(std::shared_ptr<VertexBuffer> buffer, std::shared_ptr<VertexLayout> layout);
		~glBufferLayout();

		void Bind() override;
	};

}