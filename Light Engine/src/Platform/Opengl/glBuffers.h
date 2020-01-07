#pragma once

#include "Renderer/Buffers.h"

#include "Core/Core.h"

namespace Light {

	class glVertexBuffer : public VertexBuffer
	{
	private:
		unsigned int m_BufferID = 0;
	public:
		glVertexBuffer(float* vertices, unsigned int size);
		~glVertexBuffer();

		void Bind() override;

		inline unsigned int GetID() const { return m_BufferID; }
	};

}