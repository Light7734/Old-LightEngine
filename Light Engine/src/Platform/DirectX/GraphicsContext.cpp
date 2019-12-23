#include "ltpch.h"
#include "GraphicsContext.h"

#include "Events/Event.h"
#include "Events/WindowEvents.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d10.lib")

namespace Light {

	dxGraphicsContext::dxGraphicsContext(std::shared_ptr<const Window> window_handle, WindowStyle window_style)
		: m_GameWindow(window_handle)
	{
		bool windowed = window_style != WindowStyle::ExclusiveFullscreen;

		// Create and set swap chain's description
		DXGI_SWAP_CHAIN_DESC sd = { 0 };

		sd.OutputWindow = static_cast<HWND>(window_handle->GetHandle());

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		sd.BufferDesc.Width  = NULL;
		sd.BufferDesc.Height = NULL;
		sd.BufferDesc.RefreshRate.Denominator = NULL;
		sd.BufferDesc.RefreshRate.Numerator   = NULL;
		sd.BufferDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		sd.SampleDesc.Count   = 1;
		sd.SampleDesc.Quality = 0;

		sd.Windowed = windowed;
		sd.Flags    = NULL;


		// Create device and swap chain
		D3D11CreateDeviceAndSwapChain
		(   nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			NULL,
			nullptr,
			NULL,
			D3D11_SDK_VERSION,
			&sd,
			&m_SwapChain,
			&m_Device,
			nullptr,
			&m_DeviceContext   );


		// Access the back buffer and create a render target view
		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer = nullptr;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
		m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_RenderTargetView);
	}

	void dxGraphicsContext::SwapBuffers()
	{
		m_SwapChain->Present(m_GameWindow->isVSync(), NULL);
	}

	void dxGraphicsContext::HandleWindowEvents(Event& event)
	{
		// handle m_SwapChain's Fullscreen state
		Dispatcher dispatcher(event);

		dispatcher.Dispatch<WindowMaximizedEvent>(LT_EVENT_FN(dxGraphicsContext::OnWindowMaximize));
		dispatcher.Dispatch<WindowMinimizedEvent>(LT_EVENT_FN(dxGraphicsContext::OnWindowMinimize));
		dispatcher.Dispatch<WindowRestoredEvent> (LT_EVENT_FN(dxGraphicsContext::OnWindowRestore ));
	}

	bool dxGraphicsContext::OnWindowMaximize(WindowMaximizedEvent& event)
	{
		m_SwapChain->SetFullscreenState(true, NULL);
		return false;
	}

	bool dxGraphicsContext::OnWindowMinimize(WindowMinimizedEvent& event)
	{
		m_SwapChain->SetFullscreenState(false, NULL);
		return false;
	}

	bool dxGraphicsContext::OnWindowRestore(WindowRestoredEvent& event)
	{
		m_SwapChain->SetFullscreenState(false, NULL);
		return false;
	}

}