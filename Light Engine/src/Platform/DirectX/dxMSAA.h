#pragma once

#include "Core/Core.h"

#include "Renderer/MSAA.h"

#include <wrl.h>
#include <d3d11.h>

namespace Light {

	class dxMSAA : public MSAA
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_TargetView;
	public:
		dxMSAA(unsigned int samples);

		void BindFrameBuffer() override;

		void Resolve() override;

		void Resize(unsigned int width, unsigned int height) override;
	};

}