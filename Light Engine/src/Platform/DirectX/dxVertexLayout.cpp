#include "ltpch.h"
#include "dxVertexLayout.h"

#include "dxGraphicsContext.h"
#include "dxShader.h"

#include "Debug/Exceptions.h"

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

		// shader is null, assume we only need the vertex layout and make a dummy shader
		if (!shader)
		{
			shader = Shader::Create(		    R"(+HLSL
		    float4 main(float2 p : POSITION, float2 t : TEXCOORDS) : SV_Position
		    {
		    	return float4(1.0f, 1.0f, 1.0f, 1.0f);
		    }
		    -HLSL)",
		    R"(+HLSL
		    float4 main() : SV_Target
		    {
		    	return float4(1.0f, 1.0f, 1.0f, 1.0f);
		    }-HLSL)");
		}

		std::shared_ptr<dxShader> dxpShader = std::dynamic_pointer_cast<dxShader>(shader);
		LT_CORE_ASSERT(dxpShader, "dxVertexLayout::dxVertexLayout: failed to cast Shader to dxShader");

		DXC(dxGraphicsContext::GetDevice()->CreateInputLayout(&inputElementsDesc[0],
		                                                      inputElementsDesc.size(),
		                                                      dxpShader->GetVertexBlob().Get()->GetBufferPointer(),
		                                                      dxpShader->GetVertexBlob().Get()->GetBufferSize(),
		                                                      &m_InputLayout));
	}

	void dxVertexLayout::Bind()
	{ 
		dxGraphicsContext::GetDeviceContext()->IASetInputLayout(m_InputLayout.Get());
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

}