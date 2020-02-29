#include "ltpch.h"
#include "dxBlender.h"

#include "dxGraphicsContext.h"

#include "Debug/Exceptions.h"

namespace Light {

	dxBlender::dxBlender()
	{
		// Set blend state
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr;
		DXC(dxGraphicsContext::GetDevice()->CreateBlendState(&blendDesc, &m_State));

	}

	void dxBlender::EnableImpl()
	{
		b_Enabled = true;
		dxGraphicsContext::GetDeviceContext()->OMSetBlendState(m_State.Get(), 0, 0xffffffff);
	}

	void dxBlender::DisableImpl()
	{
		b_Enabled = false;
		dxGraphicsContext::GetDeviceContext()->OMSetBlendState(nullptr, 0, 0xffffffff);
	}

}