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
	public:
		dxTextureArray(unsigned int slices);

		void CreateAtlas(const std::string& name, const std::string& texturePath, const std::string& atlasPath) override;
		void DeleteAtlas(const std::string& name);

		void Bind();
	};

}