#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

namespace Light {

	class ConstantBuffers
	{
	private:
		static std::unique_ptr<ConstantBuffers> s_Context;
	private:
		friend class GraphicsContext;
		static void Init();
	public:
		virtual ~ConstantBuffers() = default;


		static void SetViewProjMatrix(const glm::f32* view, const glm::f32* proj)
		            { s_Context->SetViewProjMatrixImpl(view, proj); }
	protected:
		virtual void SetViewProjMatrixImpl(const glm::f32* view, const glm::f32* proj) = 0;
	};

	class VertexBuffer
	{
	public:
		static std::shared_ptr<VertexBuffer> Create(float* vertices, unsigned int size, unsigned int stride);
		virtual ~VertexBuffer() = default;

		virtual void Bind() = 0;

		virtual void* Map  () = 0;
		virtual void  UnMap() = 0;
	};

	class IndexBuffer
	{
	public:
		static std::shared_ptr<IndexBuffer> Create(unsigned int* indices, unsigned int size);
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
	};

}