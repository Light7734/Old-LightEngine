#pragma once

#include "Core/Core.h"

#include "Renderer/Buffers.h"

#include <glm/glm.hpp>

namespace Light {
	
	enum UniformBufferIndex
	{
		UBufferIndex_ViewProjection = 0,
		// ...
	};

	class glUniformBuffers : public ConstantBuffers
	{
	private:
		unsigned int m_ViewProjBuffer;
	public:
		glUniformBuffers();
		~glUniformBuffers();

		void SetViewProjMatrixImpl(const glm::f32* view, const glm::f32* proj) override;
	};

	class glVertexBuffer : public VertexBuffer
	{
	private:
		unsigned int m_BufferID;
	public:
		glVertexBuffer(float* vertices, unsigned int size);
		~glVertexBuffer();

		void Bind() override;

		void* Map  () override;
		void  UnMap() override;
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