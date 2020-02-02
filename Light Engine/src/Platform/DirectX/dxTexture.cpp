#include "ltpch.h"
#include "dxTexture.h"

#include "dxGraphicsContext.h"

#include "Debug/Exceptions.h"

#include "Utility/FileManager.h"

namespace Light {

	dxTexture::dxTexture(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels)
	{
		HRESULT hr;

		LT_CORE_ASSERT(data, "Texture data is null");
		LT_CORE_ASSERT(width && height, "Width or height cannot be 0, [{}, {}]", width, height);


		// Create texture
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		D3D11_TEXTURE2D_DESC textureDesc = { 0 };

		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 0u;
		textureDesc.ArraySize = 1u;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1u;
		textureDesc.SampleDesc.Quality = 0u;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = NULL;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		DXC(dxGraphicsContext::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &texture));
		dxGraphicsContext::GetDeviceContext()->UpdateSubresource(texture.Get(), 0u, nullptr, data, sizeof(unsigned char) * width * channels, 0u);


		// Create texture view and generate mipmaps
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		DXC(dxGraphicsContext::GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, &m_TextureView));
		dxGraphicsContext::GetDeviceContext()->GenerateMips(m_TextureView.Get());

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		DXC(dxGraphicsContext::GetDevice()->CreateSamplerState(&samplerDesc, &m_SamplerState));
	}

	void dxTexture::Bind(unsigned int index/* = 0 */)
	{
		dxGraphicsContext::GetDeviceContext()->PSSetSamplers(index, 1u, m_SamplerState.GetAddressOf());
		dxGraphicsContext::GetDeviceContext()->PSSetShaderResources(index, 1u, m_TextureView.GetAddressOf());

		m_BoundSlot = index;
		b_Bound = true;
	}

}