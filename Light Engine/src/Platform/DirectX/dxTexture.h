#pragma once

#include "Core/Core.h"

#include "Renderer/Texture.h"

#include <d3d11.h>
#include <wrl.h>

namespace Light {


	class dxTextureArray : public TextureArray
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
		unsigned int m_MipLevels;

		DXGI_FORMAT m_Format;
		unsigned int m_Channels;
	public:
		dxTextureArray(unsigned int width, unsigned int height, unsigned int depth, unsigned int channels);

		void UpdateSubTexture(unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, void* pixels) override;

		void GenerateMips() override;

		void Bind(unsigned int slot = 0) override;
	};

}