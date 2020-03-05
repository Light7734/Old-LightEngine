#pragma once

#include "Core/Core.h"

namespace Light {

	class Shader;
	class VertexBuffer;
	class VertexLayout;

	class BufferLayout
	{
	public:
		virtual ~BufferLayout() = default;	

		static std::shared_ptr<BufferLayout> Create(std::shared_ptr<Shader> shader,
		                                            std::shared_ptr<VertexBuffer> buffer,
		                                            std::shared_ptr<VertexLayout> layout);

		virtual void Bind() = 0;
	};

}