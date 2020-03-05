#pragma once

#include "Core/Core.h"

#include "Renderer/Buffers.h"

#include <glm/glm.hpp>

#include <d3d11.h>
#include <wrl.h>

namespace Light {

	enum ConstantBufferIndex
	{
		CBufferIndex_ViewProjection = 0,
		// ...
	};

	class dxConstantBuffers : public ConstantBuffers
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ViewProjBuffer;
	public:
		dxConstantBuffers();

		void SetViewProjMatrixImpl(const glm::f32* view, const glm::f32* proj) override;
	};

	class dxVertexBuffer : public VertexBuffer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;

		D3D11_MAPPED_SUBRESOURCE m_Map;
		unsigned int m_Stride;
	public:
		dxVertexBuffer(float* vertices, unsigned int size, unsigned int stride);

		void* Map  () override;
		void  UnMap() override;

		void Bind() override;
	};

	class dxIndexBuffer : public IndexBuffer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
	public:
		dxIndexBuffer(unsigned int* indices, unsigned int size);

		void Bind() override;
	};

}