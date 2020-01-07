#include "ltpch.h"
#include "dxGraphicsContext.h"
	
#include "Core/Window.h"

#include "Events/Event.h"
#include "Events/WindowEvents.h"

#pragma comment(lib, "d3d11.lib")

namespace Light {

	Microsoft::WRL::ComPtr<ID3D11Device>        dxGraphicsContext::s_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> dxGraphicsContext::s_DeviceContext;

	Microsoft::WRL::ComPtr<IDXGISwapChain>         dxGraphicsContext::s_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> dxGraphicsContext::s_RenderTargetView;

	dxGraphicsContext::dxGraphicsContext(std::shared_ptr<const Window> game_window, GraphicsConfigurations data)
	{
		s_Configurations = data;
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

		sd.SampleDesc.Count   = 1u;
		sd.SampleDesc.Quality = 0u;

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
			&s_SwapChain,
			&s_Device,
			nullptr,
			&s_DeviceContext   );


		// Access the back buffer and create a render target view
		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer = nullptr;
		s_SwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &backBuffer);
		s_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &s_RenderTargetView);

		// Bind render target view and set primitive topology
		s_DeviceContext->OMSetRenderTargets(1u, s_RenderTargetView.GetAddressOf(), nullptr);
		s_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// configure viewport
		D3D11_VIEWPORT viewPort;
		viewPort.Width = game_window->GetSize().first;
		viewPort.Height = game_window->GetSize().second;
		viewPort.MinDepth = 0;
		viewPort.MaxDepth = 1;
		viewPort.TopLeftX =  0;
		viewPort.TopLeftY =  0;
		s_DeviceContext->RSSetViewports(1u, &viewPort);


		// Get the IDXGIAdapter trough IDXGIDevice
		IDXGIDevice*  DXGIDevice;
		IDXGIAdapter* adapter;

		s_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&DXGIDevice);
		DXGIDevice->GetAdapter(&adapter);
		
		// Get the adapter desc
		DXGI_ADAPTER_DESC desc;

		adapter->GetDesc(&desc);
		char DefChar = ' ';
		char ch[180];
		WideCharToMultiByte(CP_ACP, 0, desc.Description, -1, ch, 180, &DefChar, NULL);
		std::string renderer(ch);
		
		// Release memory
		DXGIDevice->Release();
		adapter->Release();

		// Log info
		LT_CORE_INFO("dxGraphicsContext:");
		LT_CORE_INFO("        Renderer: {}", renderer);
	}

	dxGraphicsContext::~dxGraphicsContext()
	{
		LT_CORE_DEBUG("Destructing dxGraphicsContext");
	}

	void dxGraphicsContext::HandleWindowEvents(Event& event)
	{
		Dispatcher dispatcher(event);

		dispatcher.Dispatch<WindowMaximizedEvent>(LT_EVENT_FN(dxGraphicsContext::OnWindowMaximize));
		dispatcher.Dispatch<WindowMinimizedEvent>(LT_EVENT_FN(dxGraphicsContext::OnWindowMinimize));
		dispatcher.Dispatch<WindowRestoredEvent> (LT_EVENT_FN(dxGraphicsContext::OnWindowRestore ));
	}

	void dxGraphicsContext::EnableVSync()
	{
		s_Configurations.vSync = true;
	}

	void dxGraphicsContext::DisableVSync()
	{
		s_Configurations.vSync = false;
	}

	void dxGraphicsContext::SwapBuffers()
	{
		s_SwapChain->Present(s_Configurations.vSync, NULL);
	}

	void dxGraphicsContext::Clear()
	{
	}

	void dxGraphicsContext::ClearBuffer(float r, float g, float b, float a)
	{
		const float channels[] = { r, g, b, a };
		s_DeviceContext->ClearRenderTargetView(s_RenderTargetView.Get(), channels);
	}


	void dxGraphicsContext::Draw(unsigned int count)
	{
		s_DeviceContext->Draw(count, 0);
	}

	bool dxGraphicsContext::OnWindowResize(WindowResizedEvent& event)
	{
		D3D11_VIEWPORT viewPort;
		viewPort.Width = event.GetWidth();
		viewPort.Height = event.GetHeight();
		viewPort.MinDepth = 0;
		viewPort.MaxDepth = 1;
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		s_DeviceContext->RSSetViewports(1u, &viewPort);

		return false;
	}

	bool dxGraphicsContext::OnWindowMaximize(WindowMaximizedEvent& event)
	{
		s_SwapChain->SetFullscreenState(true, NULL);
		return false;
	}

	bool dxGraphicsContext::OnWindowMinimize(WindowMinimizedEvent& event)
	{
		s_SwapChain->SetFullscreenState(false, NULL);
		return false;
	}

	bool dxGraphicsContext::OnWindowRestore(WindowRestoredEvent& event)
	{
		s_SwapChain->SetFullscreenState(false, NULL);
		return false;
	}

}