#include "ltpch.h"
#include "dxFramebuffer.h"

#include "dxGraphicsContext.h"

#include "Debug/Exceptions.h"

#include "Renderer/Shader.h"

namespace Light {

	dxFramebuffer::dxFramebuffer(const std::string& vertex, const std::string& pixel)
	{
		HRESULT hr;

		// Create shader 
		m_Shader = Shader::Create(vertex, pixel);

		// Create texture resource
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = GraphicsContext::GetConfigurations().resolution.width;
		textureDesc.Height = GraphicsContext::GetConfigurations().resolution.height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		DXC(dxGraphicsContext::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_Texture));

		// create the resource view on the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		DXC(dxGraphicsContext::GetDevice()->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_TextureView));

		// create the target view on the texture
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0u;
		DXC(dxGraphicsContext::GetDevice()->CreateRenderTargetView(m_Texture.Get(), &rtvDesc, &m_TargetView));
	}

	void dxFramebuffer::BindAsTarget()
	{
		float colors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		dxGraphicsContext::GetDeviceContext()->OMSetRenderTargets(1, m_TargetView.GetAddressOf(), nullptr);
		dxGraphicsContext::GetDeviceContext()->ClearRenderTargetView(m_TargetView.Get(), colors);
	}

	void dxFramebuffer::BindAsResource()
	{
		m_Shader->Bind();
		dxGraphicsContext::GetDeviceContext()->PSSetShaderResources(0, 1, m_TextureView.GetAddressOf());
	}

	void dxFramebuffer::Resize(unsigned int width, unsigned int height)
	{
		HRESULT hr;

		D3D11_TEXTURE2D_DESC texture2dDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;

		m_Texture->GetDesc(&texture2dDesc);
		m_TextureView->GetDesc(&srvDesc);
		m_TargetView->GetDesc(&rtvDesc);

		texture2dDesc.Width = width;
		texture2dDesc.Height = height;

		DXC(dxGraphicsContext::GetDevice()->CreateTexture2D(&texture2dDesc, nullptr, &m_Texture));
		DXC(dxGraphicsContext::GetDevice()->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_TextureView));
		DXC(dxGraphicsContext::GetDevice()->CreateRenderTargetView(m_Texture.Get(), &rtvDesc, &m_TargetView));

	}

}