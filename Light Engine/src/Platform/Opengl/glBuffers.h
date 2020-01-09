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

		void* Map  () override;
		void  UnMap() override;
	};

	class glIndexBuffer : public IndexBuffer
	{
	private:
		unsigned int m_BufferID = 0;
	public:
		glIndexBuffer(unsigned int* indices, unsigned int size);
		~glIndexBuffer();

		void Bind() override;
	};

}