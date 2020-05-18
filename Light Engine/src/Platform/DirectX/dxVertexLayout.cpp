#include "ltpch.h"
#include "dxVertexLayout.h"

#include "dxGraphicsContext.h"
#include "dxShader.h"

#include "Debug/Exceptions.h"

#include <d3dcompiler.h>

namespace Light {

	dxVertexLayout::dxVertexLayout(std::shared_ptr<Shader> shader, std::initializer_list<std::pair<const char*, VertexElementType>> elements)
	{
		LT_PROFILE_FUNC();

		// elements desc
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementsDesc;
		inputElementsDesc.reserve(elements.size());

		for (const auto& element : elements)
		{
			inputElementsDesc.emplace_back( D3D11_INPUT_ELEMENT_DESC{ element.first,
			                                                          0u,
			                                                          GetDxgiFormat(element.second),
			                                                          0u,
			                                                          D3D11_APPEND_ALIGNED_ELEMENT,
			                                                          D3D11_INPUT_PER_VERTEX_DATA,
			                                                          0u } );
		}


		// input layout
		HRESULT hr;

		if (!shader)
		{
			Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob = CreateDummyVertexBlob(elements);
			DXC(dxGraphicsContext::GetDevice()->CreateInputLayout(&inputElementsDesc[0],
			                                                      inputElementsDesc.size(),
			                                                      vertexBlob.Get()->GetBufferPointer(),
			                                                      vertexBlob.Get()->GetBufferSize(),
			                                                      &m_InputLayout));

		}
		else
		{
			std::shared_ptr<dxShader> dxpShader = std::dynamic_pointer_cast<dxShader>(shader);
			LT_CORE_ASSERT(dxpShader, "dxVertexLayout::dxVertexLayout: failed to cast Shader to dxShader");

			DXC(dxGraphicsContext::GetDevice()->CreateInputLayout(&inputElementsDesc[0],
			                                                      inputElementsDesc.size(),
			                                                      dxpShader->GetVertexBlob().Get()->GetBufferPointer(),
			                                                      dxpShader->GetVertexBlob().Get()->GetBufferSize(),
			                                                      &m_InputLayout));
		}
	}

	void dxVertexLayout::Bind()
	{ 
		dxGraphicsContext::GetDeviceContext()->IASetInputLayout(m_InputLayout.Get());
	}

	Microsoft::WRL::ComPtr<ID3DBlob> dxVertexLayout::CreateDummyVertexBlob(std::vector<std::pair<const char*, VertexElementType>> elements)
	{

		Microsoft::WRL::ComPtr<ID3DBlob> blob, vsErr = nullptr;
		std::string vertexSource = "float4 main(";

		for (int i = 0; i < elements.size(); i++)
		{
			vertexSource.append(GetHLSLTypeName(elements[i].second) + ' ');
			vertexSource.append('a' + std::to_string(i) + ':' + elements[i].first);

			if(i != elements.size() - 1)
				vertexSource.append(",");
		}
		vertexSource.append("):SV_Position{return float4(0,0,0,0);}");

		D3DCompile(vertexSource.c_str(), vertexSource.length(), NULL, nullptr, nullptr, "main", "vs_4_0", NULL, NULL, &blob , &vsErr);
		LT_CORE_ASSERT(!vsErr.Get(), "dxVertexLayout::CreateDummyVertexBlob: vertex shader compile error: {}", (char*)vsErr->GetBufferPointer());

		return blob;
	}

	DXGI_FORMAT dxVertexLayout::GetDxgiFormat(VertexElementType type)
	{
		switch (type)
		{
		case Light::VertexElementType::Int:       return DXGI_FORMAT_R32_SINT;
		case Light::VertexElementType::Int2:      return DXGI_FORMAT_R32G32_SINT;
		case Light::VertexElementType::Int3:      return DXGI_FORMAT_R32G32B32_SINT;
		case Light::VertexElementType::Int4:      return DXGI_FORMAT_R32G32B32A32_SINT;

		case Light::VertexElementType::UInt:      return DXGI_FORMAT_R32_UINT;
		case Light::VertexElementType::UInt2:     return DXGI_FORMAT_R32G32_UINT;
		case Light::VertexElementType::UInt3:     return DXGI_FORMAT_R32G32B32_UINT;
		case Light::VertexElementType::UInt4:     return DXGI_FORMAT_R32G32B32A32_UINT;

		case Light::VertexElementType::Float:     return DXGI_FORMAT_R32_FLOAT;
		case Light::VertexElementType::Float2:    return DXGI_FORMAT_R32G32_FLOAT;
		case Light::VertexElementType::Float3:    return DXGI_FORMAT_R32G32B32_FLOAT;
		case Light::VertexElementType::Float4:    return DXGI_FORMAT_R32G32B32A32_FLOAT;

		// #todo
		case Light::VertexElementType::Double:
		case Light::VertexElementType::Double2:
		case Light::VertexElementType::Double3:
		case Light::VertexElementType::Double4:

		default: LT_CORE_ASSERT(false, "dxVertexLayout::GetDxgiFormat: invalid vertex type");
		}
	}

	std::string dxVertexLayout::GetHLSLTypeName(VertexElementType type)
	{
		switch (type)
		{
		case Light::VertexElementType::Int:       return "int";
		case Light::VertexElementType::Int2:      return "int2";
		case Light::VertexElementType::Int3:      return "int3";
		case Light::VertexElementType::Int4:      return "int4";

		case Light::VertexElementType::UInt:      return "uint";
		case Light::VertexElementType::UInt2:     return "uint2";
		case Light::VertexElementType::UInt3:     return "uint3";
		case Light::VertexElementType::UInt4:     return "uint4";

		case Light::VertexElementType::Float:     return "float";
		case Light::VertexElementType::Float2:    return "float2";
		case Light::VertexElementType::Float3:    return "float3";
		case Light::VertexElementType::Float4:    return "float4";

		case Light::VertexElementType::Double:    return "double";
		case Light::VertexElementType::Double2:   return "double2";
		case Light::VertexElementType::Double3:	  return "double3";
		case Light::VertexElementType::Double4:   return "double4";

		default: LT_CORE_ASSERT(false, "dxVertexLayout::GetHLSLTypeName: invalid vertex type");
		}
	}

}