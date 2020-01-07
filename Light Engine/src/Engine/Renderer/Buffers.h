#pragma once

#include "Core/Core.h"

namespace Light {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		static std::shared_ptr<VertexBuffer> Create(float* vertices, unsigned int size, unsigned int stride);

		virtual void Bind() = 0;
	};

}