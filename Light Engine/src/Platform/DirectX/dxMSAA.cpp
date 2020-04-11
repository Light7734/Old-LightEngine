#include "ltpch.h"
#include "dxMSAA.h"

#include "Debug/Exceptions.h"

#include "dxGraphicsContext.h"

namespace Light {

	dxMSAA::dxMSAA(unsigned int samples)
	{
		LT_PROFILE_FUNC();

		HRESULT hr;

		UINT numQualityLevels;
		DXC(dxGraphicsContext::GetDevice()->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, samples, &numQualityLevels));

		// Create texture resource
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = GraphicsContext::GetConfigurations().resolution.width;
		textureDesc.Height = GraphicsContext::GetConfigurations().resolution.height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = samples;
		textureDesc.SampleDesc.Quality = numQualityLevels - 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		dxGraphicsContext::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_Texture);

		// create the target view on the texture
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		rtvDesc.Texture2D.MipSlice = 0u;
		DXC(dxGraphicsContext::GetDevice()->CreateRenderTargetView(m_Texture.Get(), &rtvDesc, &m_TargetView));
	}

	void dxMSAA::BindFrameBuffer()
	{
		const static const float const colors[] = { 0.0f, 0.0f, 0.0f, 0.0f };

		dxGraphicsContext::GetDeviceContext()->OMSetRenderTargets(1, m_TargetView.GetAddressOf(), nullptr);
		dxGraphicsContext::GetDeviceContext()->ClearRenderTargetView(m_TargetView.Get(), colors);
	}

	void dxMSAA::Resolve()
	{
		ID3D11RenderTargetView* rtv;
		ID3D11Resource* res;

		dxGraphicsContext::GetDeviceContext()->OMGetRenderTargets(1, &rtv, nullptr);
		rtv->GetResource(&res);

		dxGraphicsContext::GetDeviceContext()->ResolveSubresource(res, 0, m_Texture.Get(), 0, DXGI_FORMAT_R8G8B8A8_UNORM);
	}

	void dxMSAA::Resize(unsigned int width, unsigned int height)
	{
		LT_PROFILE_FUNC();

		HRESULT hr;

		D3D11_TEXTURE2D_DESC texture2dDesc;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;

		m_Texture->GetDesc(&texture2dDesc);
		m_TargetView->GetDesc(&rtvDesc);

		texture2dDesc.Width = width;
		texture2dDesc.Height = height;

		DXC(dxGraphicsContext::GetDevice()->CreateTexture2D(&texture2dDesc, nullptr, &m_Texture));
		DXC(dxGraphicsContext::GetDevice()->CreateRenderTargetView(m_Texture.Get(), &rtvDesc, &m_TargetView));
	}

}