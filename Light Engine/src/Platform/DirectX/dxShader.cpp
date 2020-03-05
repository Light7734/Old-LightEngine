#include "ltpch.h"
#include "dxShader.h"

#include "dxGraphicsContext.h"

#include "Debug/Exceptions.h"

#include <d3dcompiler.h>

namespace Light {

	dxShader::dxShader(const std::string& vertexSource, const std::string& pixelSource)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> ps = nullptr, vsErr = nullptr, psErr = nullptr;

		D3DCompile(vertexSource.c_str(), vertexSource.length(), NULL, nullptr, nullptr, "main", "vs_4_0", NULL, NULL, &m_VertexBlob, &vsErr);
		D3DCompile(pixelSource.c_str() , pixelSource.length() , NULL, nullptr, nullptr, "main", "ps_4_0", NULL, NULL, &ps, &psErr);

		LT_CORE_ASSERT(!vsErr.Get(), "dxShader::dxShader: Vertex shader compile error: {}", (char*)vsErr->GetBufferPointer());
		LT_CORE_ASSERT(!psErr.Get(), "dxShader::dxShader: Pixel shader compile error: {}", (char*)psErr->GetBufferPointer());

		HRESULT hr;
		DXC(dxGraphicsContext::GetDevice()->CreateVertexShader(m_VertexBlob->GetBufferPointer(), m_VertexBlob->GetBufferSize(), NULL, &m_VertexShader));
		DXC(dxGraphicsContext::GetDevice()->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), NULL, &m_PixelShader));
	}


	void dxShader::Bind()
	{
		dxGraphicsContext::GetDeviceContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0u);
		dxGraphicsContext::GetDeviceContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0u);
	}

}