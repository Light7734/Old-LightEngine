#pragma once

#include "Core/Core.h"

#include "Renderer/VertexLayout.h"

#include <d3d11.h>
#include <wrl.h>

namespace Light {

	class Shader;

	class dxVertexLayout : public VertexLayout
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	public:
		dxVertexLayout(std::shared_ptr<Shader> shader, std::initializer_list<std::pair<const char*, VertexElementType>> elements);

		void Bind() override;
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> CreateDummyVertexBlob(std::vector<std::pair<const char*, VertexElementType>> elements);

		DXGI_FORMAT GetDxgiFormat(VertexElementType type);
		std::string GetHLSLTypeName(VertexElementType type);
	};

}