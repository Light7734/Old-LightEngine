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
		dxTextureArray(unsigned int slices, unsigned int channels);

		// create
		void CreateSliceWithAtlas(const std::string& texturePath, const std::string& atlasName, const std::string& atlasPath) override;

		unsigned int CreateSlice(const std::string& texturePath) override;

		unsigned int CreateSlice(unsigned int width, unsigned int height, void* pixels) override;

		// delete
		void DeleteSliceWithAtlas(const std::string& atlasName) override;

		void DeleteSlice(unsigned int sliceIndex) override;

		// write
		void UpdateSubTexture(unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, void* pixels) override;
		void GenerateMips() override;


		void Bind() override;
	};

}