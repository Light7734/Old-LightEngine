#pragma once

#include "Renderer/Buffers.h"

#include "Core/Core.h"

#include <glm/glm.hpp>

#include <d3d11.h>
#include <wrl.h>

namespace Light {

	enum ConstantBufferIndex
	{
		CBufferIndex_ViewMatrix = 0,
		CBufferIndex_ProjectionMatrix = 1,
		// ...
	};

	class dxConstantBuffers : public ConstantBuffers
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ViewMatrix;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ProjectionMatrix;

		D3D11_MAPPED_SUBRESOURCE m_Map;
	public:
		dxConstantBuffers();

		void SetViewMatrixImpl(void* view) override;
		void SetProjMatrixImpl(void* proj) override;
	};

	class dxVertexBuffer : public VertexBuffer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
		D3D11_MAPPED_SUBRESOURCE m_Map;
		unsigned int m_Stride = 0u, m_Offset = 0u;
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