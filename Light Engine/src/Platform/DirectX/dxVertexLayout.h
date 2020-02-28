#pragma once

#include "Core/Core.h"

#include "Renderer/VertexLayout.h"

#include <d3d11.h>

namespace Light {

	class dxVertexLayout : public VertexLayout
	{
	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_InputElements;
	public:
		dxVertexLayout(std::initializer_list<std::pair<const char*, VertexType>> init_list);


		inline D3D11_INPUT_ELEMENT_DESC* GetElements() { return &(m_InputElements.front()); }

		inline unsigned int GetElementsCount() const { return static_cast<unsigned int>(m_InputElements.size()); }
	private:
		DXGI_FORMAT GetDxgiFormat(VertexType type);
	};

}