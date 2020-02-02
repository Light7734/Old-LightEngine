#pragma once

#include "Renderer/Texture.h"

#include "Core/Core.h"

#include <d3d11.h>
#include <wrl.h>

#include <glm/glm.hpp>

namespace Light {

	class dxTexture : public Texture
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureView;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;
	public:
		dxTexture(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels);

		void Bind(unsigned int index = 0) override;
	};

}