#pragma once

#include "Renderer/Buffers.h"

#include "Core/Core.h"

#include <d3d11.h>
#include <wrl.h>

namespace Light {

	class dxVertexBuffer : public VertexBuffer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
		unsigned int m_Stride = 0u, m_Offset = 0u;
	public:
		dxVertexBuffer(float* vertices, unsigned int size, unsigned int stride);
		~dxVertexBuffer();

		void Bind() override;
	};

	class dxIndexBuffer : public IndexBuffer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
	public:
		dxIndexBuffer(unsigned int* indices, unsigned int size);
		~dxIndexBuffer();

		void Bind() override;
	};

}