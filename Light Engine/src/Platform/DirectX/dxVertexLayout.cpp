#include "ltpch.h"
#include "dxVertexLayout.h"

namespace Light {

	dxVertexLayout::dxVertexLayout(std::initializer_list<std::pair<const char*, VertexType>> init_list)
	{
		for (const std::pair<const char*, VertexType>& listElement : init_list)
		{
			m_InputElements.push_back(D3D11_INPUT_ELEMENT_DESC());
			D3D11_INPUT_ELEMENT_DESC& inputElement = m_InputElements.back();

			inputElement.SemanticName = listElement.first;
			inputElement.Format = GetDxgiFormat(listElement.second);
			inputElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		}
	}

	DXGI_FORMAT dxVertexLayout::GetDxgiFormat(VertexType type)
	{
		switch (type)
		{
		case Light::VertexType::Int:       return DXGI_FORMAT_R32_SINT;
		case Light::VertexType::Int2:      return DXGI_FORMAT_R32G32_SINT;
		case Light::VertexType::Int3:      return DXGI_FORMAT_R32G32B32_SINT;
		case Light::VertexType::Int4:      return DXGI_FORMAT_R32G32B32A32_SINT;

		case Light::VertexType::UInt:      return DXGI_FORMAT_R32G32B32A32_UINT;
		case Light::VertexType::UInt2:     return DXGI_FORMAT_R32G32B32A32_UINT;
		case Light::VertexType::UInt3:     return DXGI_FORMAT_R32G32B32A32_UINT;
		case Light::VertexType::UInt4:     return DXGI_FORMAT_R32G32B32A32_UINT;

		case Light::VertexType::Float:     return DXGI_FORMAT_R32_FLOAT;
		case Light::VertexType::Float2:    return DXGI_FORMAT_R32G32_FLOAT;
		case Light::VertexType::Float3:    return DXGI_FORMAT_R32G32B32_FLOAT;
		case Light::VertexType::Float4:    return DXGI_FORMAT_R32G32B32A32_FLOAT;

		// #todo
		case Light::VertexType::Double:
		case Light::VertexType::Double2:
		case Light::VertexType::Double3:
		case Light::VertexType::Double4:

		default: LT_CORE_ASSERT(false, "dxVertexLayout::GetDxgiFormat: Invalid vertex type");
		}
	}

}