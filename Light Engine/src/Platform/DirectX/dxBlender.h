#pragma once

#include "Core/Core.h"

#include "Renderer/Blender.h"

#include <d3d11.h>
#include <wrl.h>

namespace Light {

	class dxBlender : public Blender
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_State;
	public:
		dxBlender();

		void EnableImpl() override;
		void DisableImpl() override;
	};

}