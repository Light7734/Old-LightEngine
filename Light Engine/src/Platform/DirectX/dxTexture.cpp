#include "ltpch.h"
#include "dxTexture.h"

#include "dxGraphicsContext.h"

#include "Debug/Exceptions.h"

#include "Utility/FileManager.h"

namespace Light {

	Microsoft::WRL::ComPtr<ID3D11Texture2D> dxTextureAtlas::s_Texture         = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dxTextureAtlas::s_SRV    = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> dxTextureAtlas::s_SamplerState = nullptr;

	unsigned int dxTextureAtlas::s_MipLevels = 0;

	dxTextureAtlas::dxTextureAtlas(const TextureData& data)
	{
		if (!s_Width && !s_Height)
			Init(data.width, data.height);

		LT_CORE_ASSERT(data.width == s_Width && data.height == s_Height,
		               "dxTextureAtlas::dxTextureAtlas: Texture slice dimensions don't match the array: slice[{}, {}] != array[{}, {}] ",
		               data.width, data.height, s_Width, s_Height);


		D3D11_TEXTURE2D_DESC textureDesc;
		s_Texture->GetDesc(&textureDesc);
		dxGraphicsContext::GetDeviceContext()->UpdateSubresource(s_Texture.Get(),
		                                                         D3D11CalcSubresource(0u, m_Index, textureDesc.MipLevels),
		                                                         nullptr,
		                                                         data.pixels,
		                                                         s_Width * 4,
		                                                         s_Width * s_Height * 4);

		dxGraphicsContext::GetDeviceContext()->GenerateMips(s_SRV.Get());
		dxGraphicsContext::GetDeviceContext()->PSSetSamplers(0u, 1u, s_SamplerState.GetAddressOf());
	}

	void dxTextureAtlas::Init(unsigned int width, unsigned int height)
	{
		HRESULT hr;

		s_Width = width;
		s_Height = height;

		// Texture descriptor
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = s_Width;
		textureDesc.Height = s_Height;
		textureDesc.MipLevels = 0u;
		textureDesc.ArraySize = 16u;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1u;
		textureDesc.SampleDesc.Quality = 0u;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = NULL;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		// Create texture
		DXC(dxGraphicsContext::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &s_Texture));
		s_Texture->GetDesc(&textureDesc);
		s_MipLevels = textureDesc.MipLevels;


		// SRV descriptor
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.MostDetailedMip = 0u;	
		srvDesc.Texture2DArray.MipLevels = -1;
		srvDesc.Texture2DArray.FirstArraySlice = 0u;
		srvDesc.Texture2DArray.ArraySize = 16u;
		

		// Create SRV and generate mips
		DXC(dxGraphicsContext::GetDevice()->CreateShaderResourceView(s_Texture.Get(), &srvDesc, &s_SRV));
		dxGraphicsContext::GetDeviceContext()->GenerateMips(s_SRV.Get());


		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		DXC(dxGraphicsContext::GetDevice()->CreateSamplerState(&samplerDesc, &s_SamplerState));

		// Bind SRV and SamplerState
		dxGraphicsContext::GetDeviceContext()->PSSetShaderResources(0u, 1u, s_SRV.GetAddressOf());
		dxGraphicsContext::GetDeviceContext()->PSSetSamplers(0u, 1u, s_SamplerState.GetAddressOf());
	}

	void dxTextureAtlas::DestroyTextureArray()
	{
		s_Texture.Reset();
		s_SRV.Reset();
		s_SamplerState.Reset();

		s_Width = 0;
		s_Height = 0;
		s_MipLevels = 0;
	}

}