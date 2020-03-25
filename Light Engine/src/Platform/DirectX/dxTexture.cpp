#include "ltpch.h"
#include "dxTexture.h"

#include "dxGraphicsContext.h"

#include "Utility/FileManager.h"

#include "Debug/Exceptions.h"

namespace Light {

	dxTextureArray::dxTextureArray(unsigned int slices)
	{
		// set depth and available slots
		m_Depth = slices;
		for (int i = slices; i > 0; i)
			m_AvailableSlots.emplace_back(--i);
	}

	void dxTextureArray::CreateAtlas(const std::string& name, const std::string& texturePath, const std::string& atlasPath)
	{
		TextureData data;
		data.pixels = FileManager::LoadTextureFile(texturePath, &data.width, &data.height, &data.channels);

		// first atlas? initialize texture array
		if (!m_Width)
		{
			HRESULT hr;

			m_Width = data.width;
			m_Height = data.height;


			// texture desc
			D3D11_TEXTURE2D_DESC textureDesc = {};
			textureDesc.Width = m_Width;
			textureDesc.Height = m_Height;
			textureDesc.MipLevels = 0u;
			textureDesc.ArraySize = 16u;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.SampleDesc.Count = 1u;
			textureDesc.SampleDesc.Quality = 0u;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			textureDesc.CPUAccessFlags = NULL;
			textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

			// create texture and store number of mip levels
			DXC(dxGraphicsContext::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_Texture));
			m_Texture->GetDesc(&textureDesc);
			m_MipLevels = textureDesc.MipLevels;


			// SRV desc
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			srvDesc.Texture2DArray.MostDetailedMip = 0u;
			srvDesc.Texture2DArray.MipLevels = textureDesc.MipLevels;
			srvDesc.Texture2DArray.FirstArraySlice = 0u;
			srvDesc.Texture2DArray.ArraySize = m_Depth;

			// create SRV and
			DXC(dxGraphicsContext::GetDevice()->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_SRV));
			}

		
		// assertions
		LT_CORE_ASSERT(data, "dxTextureArray::CreateAtlas: texture array exceeded its limit: {}", m_Depth);

		LT_CORE_ASSERT(data, "dxTextureArray::CreateAtlas: failed to load texture file: {}", texturePath)

		LT_CORE_ASSERT(data.width == m_Width && data.height == m_Height,
		               "dxTextureArray::CreateAtlas: texture dimension do not match the array's: [{} x {}] != [{} x {}]",
		               data.width, data.height, m_Width, m_Height);


		// write to the texture array and generate mips
		dxGraphicsContext::GetDeviceContext()->UpdateSubresource(m_Texture.Get(),
		                                                         D3D11CalcSubresource(0u, m_AvailableSlots.back(), m_MipLevels),
		                                                         nullptr,
		                                                         data.pixels,
		                                                         data.width * 4,
		                                                         data.width * data.height * 4);

		dxGraphicsContext::GetDeviceContext()->GenerateMips(m_SRV.Get());


		// create atlas
		m_Atlases[name] = std::make_shared<TextureAtlas>(name, atlasPath, m_AvailableSlots.back());
		m_AvailableSlots.pop_back();
	}

	void dxTextureArray::DeleteAtlas(const std::string& name)
	{
		if (m_Atlases[name])
			m_AvailableSlots.push_back(m_Atlases[name]->GetSliceIndex());
		else
			LT_CORE_ERROR("glTextureArray::DeleteAtlas: failed to find texture atlas: {}", name);

		m_Atlases.erase(name);
	}

	void dxTextureArray::Bind()
	{
		dxGraphicsContext::GetDeviceContext()->PSSetShaderResources(0u, 1u, m_SRV.GetAddressOf());
	}

}