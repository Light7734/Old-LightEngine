#include "ltpch.h"
#include "dxGraphicsContext.h"
	
#include "Events/Event.h"
#include "Events/WindowEvents.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "dxgi.lib")

#include <dxgi.h>

namespace Light {

	dxGraphicsContext::dxGraphicsContext(std::shared_ptr<const Window> game_window, bool v_sync)
	{
		b_VSync = v_sync;
		bool windowed = game_window->GetDisplayMode() != DisplayMode::ExclusiveFullscreen;

		// Create and set swap chain's description
		DXGI_SWAP_CHAIN_DESC sd = { 0 };

		sd.OutputWindow = static_cast<HWND>(game_window->GetNativeHandle());

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

		// This shit is complicated for no god damn reason
		IDXGIDevice * DXGIDevice;
		IDXGIAdapter* adapter;
		DXGI_ADAPTER_DESC desc;

		m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&DXGIDevice);
		DXGIDevice->GetAdapter(&adapter);
		adapter->GetDesc(&desc);

		char DefChar = ' ';
		char ch[180];
		WideCharToMultiByte(CP_ACP, 0, desc.Description, -1, ch, 180, &DefChar, NULL);
		std::string renderer(ch);

		LT_CORE_INFO("dxGraphicsContext:");
		LT_CORE_INFO("        Renderer: {}", renderer);
	}

	void dxGraphicsContext::SwapBuffers()
	{
		m_SwapChain->Present(b_VSync, NULL);
	}

	void dxGraphicsContext::Clear()
	{

	}

	void dxGraphicsContext::ClearBuffer(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		const float channels[] = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), channels);
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