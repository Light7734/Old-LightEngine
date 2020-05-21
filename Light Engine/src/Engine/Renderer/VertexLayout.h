#pragma once

#include "Core/Core.h"

namespace Light {

	class Shader;
	class VertexBuffer;

	enum class VertexElementType
	{
		Int,    Int2,    Int3,    Int4,
		UInt,   UInt2,   UInt3,   UInt4,
		Float,  Float2,  Float3,  Float4,
		Double, Double2, Double3, Double4
	};

	class VertexLayout
	{
	public:
		virtual ~VertexLayout() = default;

		static std::shared_ptr<VertexLayout> Create(std::shared_ptr<Shader> shader, std::shared_ptr<VertexBuffer> buffer,
		                                            const std::vector<std::pair<std::string, VertexElementType>>& elements);

		virtual void Bind() = 0;
	};

}