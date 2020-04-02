#include "ltpch.h"
#include "dxBuffers.h"

#include "dxGraphicsContext.h"

#include "Debug/Exceptions.h"

#include <glm/gtc/type_ptr.hpp>

namespace Light {
	
	// ConstantBuffers //
	dxConstantBuffer::dxConstantBuffer(ConstantBufferIndex index, unsigned int size)
		: m_Index(index)
	{
		LT_PROFILE_FUNC();

		D3D11_BUFFER_DESC bd = {};

		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.ByteWidth = size;

		HRESULT hr;
		DXC(dxGraphicsContext::GetDevice()->CreateBuffer(&bd, nullptr, &m_Buffer));

		dxGraphicsContext::GetDeviceContext()->VSSetConstantBuffers(index, 1u, m_Buffer.GetAddressOf());
		dxGraphicsContext::GetDeviceContext()->PSSetConstantBuffers(index, 1u, m_Buffer.GetAddressOf());
	}

	void dxConstantBuffer::Bind()
	{
		dxGraphicsContext::GetDeviceContext()->VSSetConstantBuffers(m_Index, 1u, m_Buffer.GetAddressOf());
		dxGraphicsContext::GetDeviceContext()->PSSetConstantBuffers(m_Index, 1u, m_Buffer.GetAddressOf());
	}

	void* dxConstantBuffer::Map()
	{
		dxGraphicsContext::GetDeviceContext()->Map(m_Buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &m_Map);
		return m_Map.pData;
	}

	void dxConstantBuffer::UnMap()
	{
		dxGraphicsContext::GetDeviceContext()->Unmap(m_Buffer.Get(), NULL);
	}

	// VertexBuffer //
	dxVertexBuffer::dxVertexBuffer(float* vertices, unsigned int size, unsigned int stride)
		: m_Stride(stride)
	{
		LT_PROFILE_FUNC();

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
		static const UINT offset = 0u;
		dxGraphicsContext::GetDeviceContext()->IASetVertexBuffers(0u, 1u, m_Buffer.GetAddressOf(), &m_Stride, &offset);
	}

	// IndexBuffer //
	dxIndexBuffer::dxIndexBuffer(unsigned int* indices, unsigned int size)
	{
		LT_PROFILE_FUNC();

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