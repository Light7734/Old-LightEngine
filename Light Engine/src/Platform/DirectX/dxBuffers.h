#pragma once

#include "Core/Core.h"

#include "Renderer/Buffers.h"

#include <glm/glm.hpp>

#include <d3d11.h>
#include <wrl.h>

namespace Light {

	class dxConstantBuffer : public ConstantBuffer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
		D3D11_MAPPED_SUBRESOURCE m_Map;

		unsigned int m_Index;
	public:
		dxConstantBuffer(ConstantBufferIndex index, unsigned int size);

		void Bind() override;

		void* Map() override;
		void UnMap() override;
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