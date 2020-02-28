#pragma once

#include "Core/Core.h"

#include "Renderer/Shader.h"

#include <d3d11.h>
#include <wrl.h>

namespace Light {

	class dxShader : public Shader
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;	

		Microsoft::WRL::ComPtr<ID3D10Blob> m_VertexBlob;
	public:
		dxShader(const std::string& vertex_source, const std::string& pixel_source);


		void Bind() override;

		Microsoft::WRL::ComPtr<ID3D10Blob> GetVertexBlob() { return m_VertexBlob; }
	};

}