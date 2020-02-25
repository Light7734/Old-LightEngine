#pragma once

#include "Renderer/GraphicsContext.h"

#include "Core/Core.h"

#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

namespace Light {

	class Event;

	class WindowResizedEvent  ;
	class WindowMaximizedEvent;
	class WindowMinimizedEvent;
	class WindowRestoredEvent ;

	class dxGraphicsContext : public GraphicsContext
	{
	private:
		static dxGraphicsContext* s_Instance;
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
	public:
		dxGraphicsContext(const GraphicsConfigurations& configurations);

		void SwapBuffers() override;

		void ClearBackbuffer(float colors[4]) override;

		void Draw(unsigned int count) override;
		void DrawIndexed(unsigned int count) override;

		// Setters
		void SetConfigurations(const GraphicsConfigurations& configurations) override;
		void SetResolution(const Resolution& resolution) override;
		void SetVSync(bool vSync) override;

		// Getters
		static ID3D11Device*        GetDevice       () { return s_Instance->m_Device.Get()       ; }
		static ID3D11DeviceContext* GetDeviceContext() { return s_Instance->m_DeviceContext.Get(); }
	private:
		bool OnWindowResize  (WindowResizedEvent&   event);
		bool OnWindowMaximize(WindowMaximizedEvent& event);
		bool OnWindowMinimize(WindowMinimizedEvent& event);
		bool OnWindowRestore (WindowRestoredEvent&  event);
	};

}