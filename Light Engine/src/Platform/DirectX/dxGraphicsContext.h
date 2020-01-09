#pragma once

#include "Renderer/GraphicsContext.h"

#include "Core/Core.h"

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

namespace Light {

	class Window;
	class Event;

	class WindowResizedEvent  ;
	class WindowMaximizedEvent;
	class WindowMinimizedEvent;
	class WindowRestoredEvent ;

	class dxGraphicsContext : public GraphicsContext
	{
	private:
		static Microsoft::WRL::ComPtr<ID3D11Device> s_Device;
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> s_DeviceContext;
		static Microsoft::WRL::ComPtr<IDXGISwapChain> s_SwapChain;
		static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> s_RenderTargetView;
	public:
		dxGraphicsContext(std::shared_ptr<const Window> game_window, GraphicsConfigurations data);

		void HandleWindowEvents(Event& event) override;

		void EnableVSync () override;
		void DisableVSync() override;

		void SwapBuffers() override;

		void Clear() override;
		void ClearBuffer(float r, float g, float b, float a) override;

		void Draw(unsigned int count) override;
		void DrawIndexed(unsigned int count) override;

		// Getters
		static ID3D11Device*        GetDevice       () { return s_Device.Get()       ; }
		static ID3D11DeviceContext* GetDeviceContext() { return s_DeviceContext.Get(); }

	private:
		bool OnWindowResize  (WindowResizedEvent&   event);
		bool OnWindowMaximize(WindowMaximizedEvent& event);
		bool OnWindowMinimize(WindowMinimizedEvent& event);
		bool OnWindowRestore (WindowRestoredEvent&  event);
	};

}