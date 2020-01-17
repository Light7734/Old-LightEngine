#pragma once

#include "Core/Core.h"

namespace Light {

	class ConstantBuffers
	{
	private:
		static std::unique_ptr<ConstantBuffers> s_Instance;
	public:
		virtual ~ConstantBuffers() = default;

		static void Init();

		static void SetViewMatrix(void* view) { s_Instance->SetViewMatrixImpl(view); }
		static void SetProjMatrix(void* proj) { s_Instance->SetProjMatrixImpl(proj); }
	protected:
		virtual void SetViewMatrixImpl(void* view) = 0;
		virtual void SetProjMatrixImpl(void* proj) = 0;
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