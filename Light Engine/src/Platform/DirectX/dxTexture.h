#pragma once

#include "Renderer/Texture.h"

#include "Core/Core.h"

#include <d3d11.h>
#include <wrl.h>

namespace Light {

	class dxTextureAtlas : public TextureAtlas
	{
	private:
		static Microsoft::WRL::ComPtr<ID3D11Texture2D> s_Texture;
		static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> s_SRV;
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> s_SamplerState;
	public:
		dxTextureAtlas(const TextureData& data);

		static void DestroyTextureArray();
	private:
		void Init(unsigned int width, unsigned int height);
	};

};