#pragma once

#include "Core/Core.h"

#include "Renderer/Blender.h"

#include <d3d11.h>
#include <wrl.h>

namespace Light {

	class dxBlender : public Blender
	{
	private:
		const std::unordered_map<BlendFactor, D3D11_BLEND> m_BlendFactorsMap;

		Microsoft::WRL::ComPtr<ID3D11BlendState> m_State;
		D3D11_BLEND_DESC m_Desc;

		BlendFactor m_SrcFactor;
		BlendFactor m_DstFactor;

		bool b_Enabled;
	public:
		dxBlender();
		~dxBlender();

		void Enable() override;
		void Disable() override;

		void SetSrcFactor(BlendFactor factor);
		void SetDstFactor(BlendFactor factor);
	};

}