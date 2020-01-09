#pragma once

#include "Core/Core.h"

namespace Light {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		static std::shared_ptr<VertexBuffer> Create(float* vertices, unsigned int size, unsigned int stride);

		virtual void Bind() = 0;

		virtual void* Map  () = 0;
		virtual void  UnMap() = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		static std::shared_ptr<IndexBuffer> Create(unsigned int* indices, unsigned int size);

		virtual void Bind() = 0;
	};

}