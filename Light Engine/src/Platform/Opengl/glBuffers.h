#pragma once

#include "Core/Core.h"

#include "Renderer/Buffers.h"

namespace Light {
	
	class glConstantBuffer : public ConstantBuffer
	{
	private:
		unsigned int m_BufferID;
		unsigned int m_Index;
	public:
		glConstantBuffer(ConstantBufferIndex index, unsigned int size);
		~glConstantBuffer();

		void Bind() override;

		void* Map() override;
		void UnMap() override;
	};

	class glVertexBuffer : public VertexBuffer
	{
	private:
		unsigned int m_BufferID;
	public:
		glVertexBuffer(float* vertices, unsigned int size);
		~glVertexBuffer();

		void Bind() override;

		void* Map() override;
		void UnMap() override;
	};

	class glIndexBuffer : public IndexBuffer
	{
	private:
		unsigned int m_BufferID;
	public:
		glIndexBuffer(unsigned int* indices, unsigned int size);
		~glIndexBuffer();

		void Bind() override;
	};

}