#include "ltpch.h"
#include "dxBuffers.h"

#include "dxGraphicsContext.h"

#include "Debug/Exceptions.h"

namespace Light {
	
	// ConstantBuffers //
	dxConstantBuffers::dxConstantBuffers()
	{
		// Create constant buffers
		D3D11_BUFFER_DESC bd = {};

		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.ByteWidth = sizeof(glm::mat4) * 2;

		HRESULT hr;
		DXC(dxGraphicsContext::GetDevice()->CreateBuffer(&bd, nullptr, &m_ViewProjBuffer));

		// Bind constant buffers
		dxGraphicsContext::GetDeviceContext()->VSSetConstantBuffers(CBufferIndex_ViewProjection,
		                                                            1u,
		                                                            m_ViewProjBuffer.GetAddressOf());
	}


	void dxConstantBuffers::SetViewProjMatrixImpl(const glm::f32* view, const glm::f32* proj)
	{
		D3D11_MAPPED_SUBRESOURCE map;
		dxGraphicsContext::GetDeviceContext()->Map(m_ViewProjBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &map);

		memcpy(map.pData, view, sizeof(glm::mat4));
		map.pData = (glm::mat4*)map.pData + 1;
		memcpy(map.pData, proj, sizeof(glm::mat4));

		dxGraphicsContext::GetDeviceContext()->Unmap(m_ViewProjBuffer.Get(), NULL);
	}


	// VertexBuffer //
	dxVertexBuffer::dxVertexBuffer(float* vertices, unsigned int size, unsigned int stride)
		: m_Stride(stride)
	{
		D3D11_BUFFER_DESC bd = {};
		D3D11_SUBRESOURCE_DATA sd = {};

		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage     = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.ByteWidth = size;
		bd.StructureByteStride = stride;

		sd.pSysMem = vertices;

		HRESULT hr;
		DXC(dxGraphicsContext::GetDevice()->CreateBuffer(&bd, sd.pSysMem ? &sd : nullptr, &m_Buffer));
	}


	void* dxVertexBuffer::Map()
	{
		dxGraphicsContext::GetDeviceContext()->Map(m_Buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &m_Map);
		return m_Map.pData;
	}

	void dxVertexBuffer::UnMap()
	{
		dxGraphicsContext::GetDeviceContext()->Unmap(m_Buffer.Get(), NULL);
	}

	void dxVertexBuffer::Bind()
	{
		dxGraphicsContext::GetDeviceContext()->IASetVertexBuffers(0u, 1u, m_Buffer.GetAddressOf(), &m_Stride, &m_Offset);
	}


	// IndexBuffer //
	dxIndexBuffer::dxIndexBuffer(unsigned int* indices, unsigned int size)
	{
		D3D11_BUFFER_DESC bd = {};
		D3D11_SUBRESOURCE_DATA sd = {};

		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage     = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = size;
		bd.StructureByteStride = sizeof(unsigned int);

		sd.pSysMem = indices;

		HRESULT hr;
		DXC(dxGraphicsContext::GetDevice()->CreateBuffer(&bd, &sd, &m_Buffer));
	}


	void dxIndexBuffer::Bind()
	{
		dxGraphicsContext::GetDeviceContext()->IASetIndexBuffer(m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	}

}