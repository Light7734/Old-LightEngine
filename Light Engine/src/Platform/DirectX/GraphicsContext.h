#pragma once

#include "Renderer/GraphicsContext.h"

#include "Core/Core.h"
#include "Core/Window.h"

#include <Windows.h>

#include <d3d11.h>
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
		std::shared_ptr<const Window> m_GameWindow;

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
	public:
		dxGraphicsContext(std::shared_ptr<const Window> window_handle, WindowStyle window_style);

		void SwapBuffers() override;
		void HandleWindowEvents(Event& event) override;
	private:
		bool OnWindowMaximize(WindowMaximizedEvent& event);
		bool OnWindowMinimize(WindowMinimizedEvent& event);
		bool OnWindowRestore (WindowRestoredEvent&  event);
	};

}