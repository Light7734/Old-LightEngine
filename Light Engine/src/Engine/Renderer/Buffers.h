#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

namespace Light {

	class ConstantBuffers
	{
	private:
		static std::unique_ptr<ConstantBuffers> s_Context;
	public:
		virtual ~ConstantBuffers() = default;

		static void SetViewProjMatrix(const glm::f32* view, const glm::f32* proj)
		            { s_Context->SetViewProjMatrixImpl(view, proj); }

		virtual void SetViewProjMatrixImpl(const glm::f32* view, const glm::f32* proj) = 0;
	private:
		friend class GraphicsContext;
		static void Init();
	};

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