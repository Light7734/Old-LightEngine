#pragma once

#include "Core/Core.h"

namespace Light {

	enum ConstantBufferIndex
	{
		// Slots for client side
		ConstantBufferIndex_ClientSlot0 = 0,
		ConstantBufferIndex_ClientSlot1 = 1,
		ConstantBufferIndex_ClientSlot2 = 2,
		ConstantBufferIndex_ClientSlot3 = 3,
		ConstantBufferIndex_ClientSlot4 = 4,
		ConstantBufferIndex_ClientSlot5 = 5,

		// Slots for engine
		ConstantBufferIndex_ViewProjection = 6,
	};

	class ConstantBuffer
	{
	public:
		virtual ~ConstantBuffer() = default;

		static std::shared_ptr<ConstantBuffer> Create(ConstantBufferIndex index, unsigned int size);

		virtual void Bind() = 0;

		virtual void* Map() = 0;
		virtual void UnMap() = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		static std::shared_ptr<VertexBuffer> Create(float* vertices, unsigned int stride, unsigned int count);

		virtual void Bind() = 0;

		virtual void* Map() = 0;
		virtual void UnMap() = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		static std::shared_ptr<IndexBuffer> Create(unsigned int* indices, unsigned int count);

		virtual void Bind() = 0;
	};

}