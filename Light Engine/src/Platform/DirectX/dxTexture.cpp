#include "ltpch.h"
#include "dxTexture.h"

#include "dxGraphicsContext.h"

#include "Debug/Exceptions.h"

#include "Utility/FileManager.h"

namespace Light {

	dxTextureArray::dxTextureArray(unsigned int slices, unsigned int channels)
		: m_Channels(channels)
	{
		m_Format =
			m_Channels == 1 ? DXGI_FORMAT_R8_UNORM       : 
			m_Channels == 2 ? DXGI_FORMAT_R8G8_UNORM     :
			m_Channels == 3 ? DXGI_FORMAT_R8G8B8A8_UNORM :
			m_Channels == 4 ? DXGI_FORMAT_R8G8B8A8_UNORM :
			                  DXGI_FORMAT_UNKNOWN        ;

		// set depth and available slots
		m_Depth = slices;
		for (int i = slices; i > 0; i)
			m_AvailableSlots.emplace_back(--i);
	}

	void dxTextureArray::CreateSliceWithAtlas(const std::string& texturePath, const std::string& atlasName, const std::string& atlasPath)
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
			textureDesc.Format = m_Format;
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
			srvDesc.Format = m_Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			srvDesc.Texture2DArray.MostDetailedMip = 0u;
			srvDesc.Texture2DArray.MipLevels = textureDesc.MipLevels;
			srvDesc.Texture2DArray.FirstArraySlice = 0u;
			srvDesc.Texture2DArray.ArraySize = m_Depth;

			// create SRV and
			DXC(dxGraphicsContext::GetDevice()->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_SRV));
		}

		// assertions
		LT_CORE_ASSERT(!m_AvailableSlots.empty(), "dxTextureArray::CreateSliceWithAtlas: texture array exceeded its limit: {}", m_Depth);

		LT_CORE_ASSERT(data, "dxTextureArray::CreateSliceWithAtlas: failed to load texture file: {}", texturePath)

		LT_CORE_ASSERT(data.width == m_Width && data.height == m_Height,
		               "dxTextureArray::CreateSliceWithAtlas: texture dimension do not match the array's: [{} x {}] != [{} x {}]",
		               data.width, data.height, m_Width, m_Height);

		// write to the texture array and generate mips
		UpdateSubTexture(0, 0, m_AvailableSlots.back(), m_Width, m_Height, data.pixels);
		GenerateMips();

		// create atlas
		m_Atlases[atlasName] = std::make_shared<TextureAtlas>(atlasName, atlasPath, m_AvailableSlots.back());
		m_AvailableSlots.pop_back();
	}

	unsigned int dxTextureArray::CreateSlice(const std::string& texturePath)
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
			textureDesc.Format = m_Format;
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
			srvDesc.Format = m_Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			srvDesc.Texture2DArray.MostDetailedMip = 0u;
			srvDesc.Texture2DArray.MipLevels = textureDesc.MipLevels;
			srvDesc.Texture2DArray.FirstArraySlice = 0u;
			srvDesc.Texture2DArray.ArraySize = m_Depth;

			// create SRV and
			DXC(dxGraphicsContext::GetDevice()->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_SRV));
		}

		// assertions
		LT_CORE_ASSERT(!m_AvailableSlots.empty(), "dxTextureArray::CreateSlice: texture array exceeded its limit: {}", m_Depth);

		LT_CORE_ASSERT(data, "dxTextureArray::CreateSlice: failed to load texture file: {}", texturePath)

		LT_CORE_ASSERT(data.width == m_Width && data.height == m_Height,
		               "dxTextureArray::CreateSlice: texture dimension do not match the array's: [{} x {}] != [{} x {}]",
		               data.width, data.height, m_Width, m_Height);

		// store, then pop selected index
		unsigned int sliceIndex = m_AvailableSlots.back();
		m_AvailableSlots.pop_back();

		// write to the texture array and generate mips
		UpdateSubTexture(0, 0, m_AvailableSlots.back(), m_Width, m_Height, data.pixels);
		GenerateMips();

		return  sliceIndex;
	}

	unsigned int dxTextureArray::CreateSlice(unsigned int width, unsigned int height, void* pixels)
	{
		// first atlas? initialize texture array
		if (!m_Width)
		{
			HRESULT hr;

			m_Width = width;
			m_Height = height;

			// texture desc
			D3D11_TEXTURE2D_DESC textureDesc = {};
			textureDesc.Width = m_Width;
			textureDesc.Height = m_Height;
			textureDesc.MipLevels = 0u;
			textureDesc.ArraySize = 16u;
			textureDesc.Format = m_Format;
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

			srvDesc.Format = m_Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			srvDesc.Texture2DArray.MostDetailedMip = 0u;
			srvDesc.Texture2DArray.MipLevels = textureDesc.MipLevels;
			srvDesc.Texture2DArray.FirstArraySlice = 0u;
			srvDesc.Texture2DArray.ArraySize = m_Depth;

			// create SRV and
			DXC(dxGraphicsContext::GetDevice()->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_SRV));
		}

		// assertions
		LT_CORE_ASSERT(!m_AvailableSlots.empty(), "dxTextureArray::CreateSlice: texture array exceeded its limit: {}", m_Depth);

		LT_CORE_ASSERT(width == m_Width && height == m_Height,
		               "dxTextureArray::CreateSlice: assigned dimension do not match the array's: [{} x {}] != [{} x {}]",
		               width, height, m_Width, m_Height);

		// store, then pop selected index
		unsigned int sliceIndex = m_AvailableSlots.back();
		m_AvailableSlots.pop_back();

		// write to the texture array and generate mips
		if (pixels)
		{
			UpdateSubTexture(0, 0, m_AvailableSlots.back(), m_Width, m_Height, pixels);
			GenerateMips();
		}

		return sliceIndex;
	}

	void dxTextureArray::DeleteSliceWithAtlas(const std::string& name)
	{
		if (m_Atlases[name])
			m_AvailableSlots.push_back(m_Atlases[name]->GetSliceIndex());
		else
			LT_CORE_ERROR("glTextureArray::DeleteAtlas: failed to find texture atlas: {}", name);

		m_Atlases.erase(name);
	}

	void dxTextureArray::DeleteSlice(unsigned int sliceIndex)
	{
		m_AvailableSlots.push_back(sliceIndex);
	}

	void dxTextureArray::UpdateSubTexture(unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, void* pixels)
	{
		D3D11_BOX box;
		box.left = xoffset;
		box.right = xoffset + width;
		box.top = yoffset;
		box.bottom = yoffset + height;
		box.front = zoffset;
		box.back = zoffset + 1;

		dxGraphicsContext::GetDeviceContext()->UpdateSubresource(m_Texture.Get(),
		                                                         D3D11CalcSubresource(0u, zoffset, m_MipLevels),
		                                                         &box,
		                                                         pixels,
		                                                         width * m_Channels,
		                                                         width * height * m_Channels);
	}

	void dxTextureArray::GenerateMips()
	{
		dxGraphicsContext::GetDeviceContext()->GenerateMips(m_SRV.Get());
	}

	void dxTextureArray::Bind()
	{
		dxGraphicsContext::GetDeviceContext()->PSSetShaderResources(0u, 1u, m_SRV.GetAddressOf());
	}

}


// 		dxGraphicsContext::GetDeviceContext()->UpdateSubresource(m_Texture.Get(),
// 		                                                         D3D11CalcSubresource(0u, m_AvailableSlots.back(), m_MipLevels),
// 		                                                         nullptr,
// 		                                                         data.pixels,
// 		                                                         data.width * 4,
// 		                                                         data.width * data.height * 4);
// 
// 		dxGraphicsContext::GetDeviceContext()->GenerateMips(m_SRV.Get());
