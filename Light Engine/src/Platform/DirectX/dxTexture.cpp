#include "ltpch.h"
#include "dxTexture.h"

#include "dxGraphicsContext.h"

#include "Debug/Exceptions.h"

#include "Utility/FileManager.h"

namespace Light {

	Microsoft::WRL::ComPtr<ID3D11Texture2D> dxTextureAtlas::s_Texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> dxTextureAtlas::s_SRV;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> dxTextureAtlas::s_SamplerState;

	dxTextureAtlas::dxTextureAtlas(const TextureData& data)
	{
		if (!s_Width && !s_Height)
			Init(data.width, data.height);

		LT_CORE_ASSERT(data.width == s_Width && data.height == s_Height,
		               "Texture slice dimensions does not match the array: slice[{}, {}] != array[{}, {}] ",
		               data.width, data.height, s_Width, s_Height);


		D3D11_TEXTURE2D_DESC desc;
		s_Texture->GetDesc(&desc);
		dxGraphicsContext::GetDeviceContext()->UpdateSubresource(s_Texture.Get(),
		                                                         D3D11CalcSubresource(0u, m_Index, desc.MipLevels),
		                                                         nullptr,
		                                                         data.pixels,
		                                                         s_Width * 4,
		                                                         NULL);
		dxGraphicsContext::GetDeviceContext()->GenerateMips(s_SRV.Get());
	}

	void dxTextureAtlas::Init(unsigned int width, unsigned int height)
	{
		HRESULT hr;

		s_Width = width;
		s_Height = height;

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

		DXC(dxGraphicsContext::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &s_Texture));


		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.MostDetailedMip = 0u;	
		srvDesc.Texture2DArray.MipLevels = -1;
		srvDesc.Texture2DArray.FirstArraySlice = 0u;
		srvDesc.Texture2DArray.ArraySize = 16u;
		

		DXC(dxGraphicsContext::GetDevice()->CreateShaderResourceView(s_Texture.Get(), &srvDesc, &s_SRV));
		dxGraphicsContext::GetDeviceContext()->GenerateMips(s_SRV.Get());

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
	}

}