#include "ltpch.h"
#include "dxTexture.h"

#include "dxGraphicsContext.h"

#include "Debug/Exceptions.h"

#include "Utility/FileManager.h"

namespace Light {

	dxTextureArray::dxTextureArray(unsigned int width, unsigned int height, unsigned int depth, unsigned int channels)
		: TextureArray(width, height, depth, channels)
	{
		LT_PROFILE_FUNC();
		HRESULT hr;

		m_Format = channels == 4 ? DXGI_FORMAT_R8G8B8A8_UNORM :
		           channels == 2 ? DXGI_FORMAT_R8G8_UNORM     :
		           channels == 1 ? DXGI_FORMAT_R8_UNORM       :
		                           DXGI_FORMAT_UNKNOWN        ;

		D3D11_TEXTURE2D_DESC textureDesc;

		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 0u;
		textureDesc.ArraySize = depth;
		textureDesc.Format = m_Format;
		textureDesc.SampleDesc.Count = 1u;
		textureDesc.SampleDesc.Quality = 0u;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = NULL;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		DXC(dxGraphicsContext::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_Texture));
		m_Texture->GetDesc(&textureDesc);
		m_MipLevels = textureDesc.MipLevels;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = m_Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.ArraySize = depth;
		srvDesc.Texture2DArray.FirstArraySlice = 0u;
		srvDesc.Texture2DArray.MipLevels = m_MipLevels;
		srvDesc.Texture2DArray.MostDetailedMip = 0u;

		DXC(dxGraphicsContext::GetDevice()->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_SRV));
	}

	dxTextureArray::~dxTextureArray()
	{
		LT_PROFILE_FUNC();

		ID3D11ShaderResourceView* srv = { nullptr };
		dxGraphicsContext::GetDeviceContext()->PSSetShaderResources(m_BoundSlot, 1u, &srv);
	}

	void dxTextureArray::UpdateSubTexture(unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, void* pixels)
	{
		LT_PROFILE_FUNC();

		D3D11_BOX box;
		box.left = xoffset;
		box.right = xoffset + width;
		box.top = yoffset;
		box.bottom = yoffset + height;
		box.front = zoffset;
		box.back = zoffset + 1;

		dxGraphicsContext::GetDeviceContext()->UpdateSubresource(m_Texture.Get(),
		                                                         D3D11CalcSubresource(0, zoffset, m_MipLevels),
		                                                         &box,
		                                                         pixels,
		                                                         width * m_Channels,
		                                                         width * height * m_Channels);
	}

	void dxTextureArray::UpdateSubTexture(const TextureCoordinates& bounds, void* pixels)
	{
		LT_PROFILE_FUNC();

		D3D11_BOX box;
		box.left = bounds.xMin;
		box.right = bounds.xMax;
		box.top = bounds.yMin;
		box.bottom = bounds.yMax;
		box.front = bounds.sliceIndex;
		box.back = bounds.sliceIndex + 1;

		dxGraphicsContext::GetDeviceContext()->UpdateSubresource(m_Texture.Get(),
		                                                         D3D11CalcSubresource(0, bounds.sliceIndex, m_MipLevels),
		                                                         &box,
		                                                         pixels,
		                                                         (bounds.xMax - bounds.xMin) * m_Channels,
		                                                         (bounds.xMax - bounds.xMin) * (bounds.yMax - bounds.yMin) * m_Channels);
	}

	void dxTextureArray::GenerateMips()
	{
		LT_PROFILE_FUNC();
		dxGraphicsContext::GetDeviceContext()->GenerateMips(m_SRV.Get());
	}

	void dxTextureArray::Bind(unsigned int slot	/* = 0 */)
	{
		LT_PROFILE_FUNC();

		m_BoundSlot = slot;	
		dxGraphicsContext::GetDeviceContext()->PSSetShaderResources(slot, 1u, m_SRV.GetAddressOf());
	}

}