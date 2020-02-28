#pragma once

#include "Core/Core.h"

#include "Renderer/BufferLayout.h"

#include <d3d11.h>
#include <wrl.h>

namespace Light {

	class Shader;
	class VertexLayout;

	class dxBufferLayout : public BufferLayout
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	public:
		dxBufferLayout(std::shared_ptr<Shader> shader, std::shared_ptr<VertexLayout> layout);


		void Bind() override;
	};

}