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
		dxVertexLayout(std::shared_ptr<Shader> shader, const std::vector<std::pair<std::string, VertexElementType>>& elements);

		void Bind() override;
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> CreateDummyVertexBlob(const std::vector<std::pair<std::string, VertexElementType>>& elements);

		DXGI_FORMAT GetDxgiFormat(VertexElementType type);
		std::string GetHLSLTypeName(VertexElementType type);
	};

}