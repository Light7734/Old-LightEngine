#include "ltpch.h"
#include "dxShader.h"

#include "dxGraphicsContext.h"

#include "Debug/Exceptions.h"

#include <d3dcompiler.h>

namespace Light {

	dxShader::dxShader(const std::string& vertexSource, const std::string& pixelSource)
	{
		LT_PROFILE_FUNC();

		Microsoft::WRL::ComPtr<ID3DBlob> ps = nullptr, vsErr = nullptr, psErr = nullptr;

		D3DCompile(vertexSource.c_str(), vertexSource.length(), NULL, nullptr, nullptr, "main", "vs_4_0", NULL, NULL, &m_VertexBlob, &vsErr);
		D3DCompile(pixelSource.c_str() , pixelSource.length() , NULL, nullptr, nullptr, "main", "ps_4_0", NULL, NULL, &ps, &psErr);

		LT_CORE_ASSERT(!vsErr.Get(), "dxShader::dxShader: vertex shader compile error: {}", (char*)vsErr->GetBufferPointer());
		LT_CORE_ASSERT(!psErr.Get(), "dxShader::dxShader: pixel shader compile error: {}", (char*)psErr->GetBufferPointer());

		HRESULT hr;
		DXC(dxGraphicsContext::GetDevice()->CreateVertexShader(m_VertexBlob->GetBufferPointer(), m_VertexBlob->GetBufferSize(), NULL, &m_VertexShader));
		DXC(dxGraphicsContext::GetDevice()->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), NULL, &m_PixelShader));

		ExtractVertexElements(vertexSource);
	}

	dxShader::~dxShader()
	{
		LT_PROFILE_FUNC();

		ID3D11VertexShader* vs = nullptr;
		ID3D11PixelShader* ps = nullptr;
		
		dxGraphicsContext::GetDeviceContext()->VSSetShader(vs, nullptr, 0u);
		dxGraphicsContext::GetDeviceContext()->PSSetShader(ps, nullptr, 0u);
	}

	void dxShader::Bind()
	{
		dxGraphicsContext::GetDeviceContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0u);
		dxGraphicsContext::GetDeviceContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0u);
	}

	void dxShader::ExtractVertexElements(const std::string& vertexSource)
	{
		LT_PROFILE_FUNC();

		std::string line = vertexSource.substr(vertexSource.find("main(") + 5);
		line = line.substr(0, line.find(')'));

		std::istringstream lineStream(line);
		std::string token;

		while (std::getline(lineStream, token, ' '))
		{
			VertexElementType type = GetVertexElementType(token.c_str());

			std::getline(lineStream, token, ' ');
			std::getline(lineStream, token, ' ');	 
			std::getline(lineStream, token, ' ');

			if(token.find(',') != std::string::npos)
				token = token.substr(0, token.length() - 1);

			m_Elements.push_back({ token, type });
		}
	}	

	VertexElementType dxShader::GetVertexElementType(const char* typeName)
	{
		switch (hashStr(typeName))
		{
		case hashStr("int"):       return VertexElementType::Int;
		case hashStr("int2"):     return VertexElementType::Int2;
		case hashStr("int3"):     return VertexElementType::Int3;
		case hashStr("int4"):     return VertexElementType::Int4;

		case hashStr("uint"):      return VertexElementType::UInt;
		case hashStr("uint2"):     return VertexElementType::UInt2;
		case hashStr("uint3"):     return VertexElementType::UInt3;
		case hashStr("uint4"):     return VertexElementType::UInt4;

		case hashStr("float"):     return VertexElementType::Float;
		case hashStr("float2"):      return VertexElementType::Float2;
		case hashStr("float3"):      return VertexElementType::Float3;
		case hashStr("float4"):      return VertexElementType::Float4;

		case hashStr("double"):    return VertexElementType::Double;
		case hashStr("double2"):     return VertexElementType::Double2;
		case hashStr("double3"):     return VertexElementType::Double3;
		case hashStr("double4"):     return VertexElementType::Double4;

		default: LT_CORE_ASSERT(false, "dxShader::GetElementType: invalid typeName");
		}
	}

}