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
	public:
		dxBlender();

		void Enable() override;
		void Disable() override;

		void SetSrcFactor(BlendFactor factor);
		void SetDstFactor(BlendFactor factor);
	};

}