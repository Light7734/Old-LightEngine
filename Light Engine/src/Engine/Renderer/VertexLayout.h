#pragma once

#include "Core/Core.h"

namespace Light {

	enum class VertexType
	{
		Int,    Int2,    Int3,    Int4,
		UInt,   UInt2,   UInt3,   UInt4,
		Float,  Float2,  Float3,  Float4,
		Double, Double2, Double3, Double4
	};

	class VertexLayout
	{
	public:
		static std::shared_ptr<VertexLayout> Create(std::initializer_list<std::pair<const char*, VertexType>> init_list);
		virtual ~VertexLayout() = default;
	};

}