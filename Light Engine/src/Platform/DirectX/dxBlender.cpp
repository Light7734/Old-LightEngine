#include "ltpch.h"
#include "dxBlender.h"

#include "dxGraphicsContext.h"

#include "Debug/Exceptions.h"

namespace Light {

	dxBlender::dxBlender()
		: m_BlendFactorsMap({
			{ BlendFactor::ZERO, D3D11_BLEND_ZERO },
			{ BlendFactor::ONE , D3D11_BLEND_ONE  },
			
			{ BlendFactor::SRC_COLOR        , D3D11_BLEND_SRC_COLOR     },
			{ BlendFactor::SRC_COLOR_INVERSE, D3D11_BLEND_INV_SRC_COLOR },
			{ BlendFactor::SRC_ALPHA        , D3D11_BLEND_SRC_ALPHA     },
			{ BlendFactor::SRC_ALPHA_INVERSE, D3D11_BLEND_INV_SRC_ALPHA },
			
			{ BlendFactor::DST_COLOR        , D3D11_BLEND_DEST_COLOR     },
			{ BlendFactor::DST_COLOR_INVERSE, D3D11_BLEND_INV_DEST_COLOR },
			{ BlendFactor::DST_ALPHA        , D3D11_BLEND_DEST_ALPHA     },
			{ BlendFactor::DST_ALPHA_INVERSE, D3D11_BLEND_INV_DEST_ALPHA }})
	{
		LT_PROFILE_FUNC();

		// Set blend state
		m_Desc = {};
		m_Desc.RenderTarget[0].BlendEnable = true;
		m_Desc.RenderTarget[0].SrcBlend = m_BlendFactorsMap.at(s_SrcFactor);
		m_Desc.RenderTarget[0].DestBlend = m_BlendFactorsMap.at(s_DstFactor);
		m_Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		m_Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		m_Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		m_Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		m_Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr;
		DXC(dxGraphicsContext::GetDevice()->CreateBlendState(&m_Desc, &m_State));
	}

	void dxBlender::EnableImpl()
	{
		dxGraphicsContext::GetDeviceContext()->OMSetBlendState(m_State.Get(), 0, 0xffffffff);
	}

	void dxBlender::DisableImpl()
	{
		dxGraphicsContext::GetDeviceContext()->OMSetBlendState(nullptr, 0, 0xffffffff);
	}

	void dxBlender::SetSrcFactorImpl(BlendFactor factor)
	{
		m_Desc.RenderTarget->SrcBlend = m_BlendFactorsMap.at(factor);
		dxGraphicsContext::GetDevice()->CreateBlendState(&m_Desc, &m_State);

		if(b_Enabled)
			dxGraphicsContext::GetDeviceContext()->OMSetBlendState(m_State.Get(), 0, 0xffffffff);
	}

	void dxBlender::SetDstFactorImpl(BlendFactor factor)
	{
		m_Desc.RenderTarget->DestBlend = m_BlendFactorsMap.at(factor);
		dxGraphicsContext::GetDevice()->CreateBlendState(&m_Desc, &m_State);

		if (b_Enabled)
			dxGraphicsContext::GetDeviceContext()->OMSetBlendState(m_State.Get(), 0, 0xffffffff);
	}

}