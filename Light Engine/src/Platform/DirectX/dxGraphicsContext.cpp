#include "ltpch.h"
#include "dxGraphicsContext.h"
	
#include "Core/Window.h"

#include "Events/Event.h"
#include "Events/WindowEvents.h"

namespace Light {

	dxGraphicsContext* dxGraphicsContext::s_Instance = nullptr;

	dxGraphicsContext::dxGraphicsContext(const GraphicsConfigurations& configurations) 
	{
		m_Configurations = configurations;
		s_Instance = this;

		bool windowed = Window::GetDisplayMode() != DisplayMode::Fullscreen;

		// Create and set swap chain's description
		DXGI_SWAP_CHAIN_DESC sd = { 0 };

		sd.OutputWindow = static_cast<HWND>(Window::GetNativeHandle());

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1u;

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
			&m_SwapChain,
			&m_Device,
			nullptr,
			&m_DeviceContext   );
		m_SwapChain->SetFullscreenState(!windowed, nullptr);
		

		// Access the back buffer and create a render target view
		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer = nullptr;
		m_SwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &backBuffer);
		m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_RenderTargetView);

		// Bind render target view and set primitive topology
		m_DeviceContext->OMSetRenderTargets(1u, m_RenderTargetView.GetAddressOf(), nullptr);
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// Set rasterizer viewport
		D3D11_VIEWPORT viewPort;
		viewPort.Width = static_cast<float>(Window::GetWidth());
		viewPort.Height = static_cast<float>(Window::GetHeight());
		viewPort.MinDepth = 0;
		viewPort.MaxDepth = 1;
		viewPort.TopLeftX =  0;
		viewPort.TopLeftY =  0;
		m_DeviceContext->RSSetViewports(1u, &viewPort);

		
		// Get the IDXGIAdapter trough IDXGIDevice
		IDXGIDevice*  DXGIDevice;
		IDXGIAdapter* adapter;

		m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&DXGIDevice);
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
		m_SwapChain->SetFullscreenState(false, nullptr);
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
		m_Configurations.vSync = true;
	}

	void dxGraphicsContext::DisableVSync()
	{
		m_Configurations.vSync = false;
	}

	void dxGraphicsContext::SwapBuffers()
	{
		m_SwapChain->Present(m_Configurations.vSync, NULL);
	}

	void dxGraphicsContext::Clear()
	{
	}

	void dxGraphicsContext::ClearBuffer(float r, float g, float b, float a)
	{
		const float channels[] = { r, g, b, a };
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), channels);
	}

	void dxGraphicsContext::Draw(unsigned int count)
	{
		m_DeviceContext->Draw(count, 0u);
	}

	void dxGraphicsContext::DrawIndexed(unsigned int count)
	{
		m_DeviceContext->DrawIndexed(count, 0u, 0u);
	}

	bool dxGraphicsContext::OnWindowResize(WindowResizedEvent& event)
	{
		D3D11_VIEWPORT viewPort;
		viewPort.Width = static_cast<float>(event.GetWidth());
		viewPort.Height = static_cast<float>(event.GetHeight());
		viewPort.MinDepth = 0;
		viewPort.MaxDepth = 1;
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		m_DeviceContext->RSSetViewports(1u, &viewPort);

		return false;
	}

	bool dxGraphicsContext::OnWindowMaximize(WindowMaximizedEvent& event)
	{
		m_SwapChain->SetFullscreenState(true, nullptr);
		return false;
	}

	bool dxGraphicsContext::OnWindowMinimize(WindowMinimizedEvent& event)
	{
		m_SwapChain->SetFullscreenState(false, nullptr);
		return false;
	}

	bool dxGraphicsContext::OnWindowRestore(WindowRestoredEvent& event)
	{
		m_SwapChain->SetFullscreenState(false, nullptr);
		return false;
	}

}