	#include "ltpch.h"
#include "dxGraphicsContext.h"
	
#include "Core/Window.h"

#include "Debug/Exceptions.h"

#include "Events/Event.h"
#include "Events/WindowEvents.h"

#include <glfw/glfw3.h>

namespace Light {

	dxGraphicsContext* dxGraphicsContext::s_Instance = nullptr;

	dxGraphicsContext::dxGraphicsContext(const GraphicsConfigurations& configurations) 
	{
		HRESULT hr;

		m_Configurations = configurations;
		s_Instance = this;

		// Create swap chain's description
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

		sd.Windowed = Window::GetDisplayMode() != DisplayMode::Fullscreen;
		sd.Flags    = NULL;


		// Create device and swap chain
		DXC( D3D11CreateDeviceAndSwapChain
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
			&m_DeviceContext   ));


		// Access the back buffer and create a render target view
		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer = nullptr;
		DXC(m_SwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &backBuffer));
		DXC(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_RenderTargetView));

		// Bind render target view and set primitive topology
		m_DeviceContext->OMSetRenderTargets(1u, m_RenderTargetView.GetAddressOf(), nullptr);
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// Set rasterizer viewport
		D3D11_VIEWPORT viewPort;
		viewPort.Width  = static_cast<float>(Window::GetWidth ());
		viewPort.Height = static_cast<float>(Window::GetHeight());
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = 0.0f;
		viewPort.TopLeftY = 0.0f;
		m_DeviceContext->RSSetViewports(1u, &viewPort);

		
		// Locals 
		IDXGIDevice*      DXGIDevice;
		IDXGIAdapter*     DXGIAdapter;
		DXGI_ADAPTER_DESC DXGIAdapterDesc;

		// Initialize Locals
		DXC(m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&DXGIDevice));
		DXC(DXGIDevice->GetAdapter(&DXGIAdapter));
		DXC(DXGIAdapter->GetDesc(&DXGIAdapterDesc));

		// Get the Adapter's Description
		char DefChar = ' ';
		char ch[180];
		WideCharToMultiByte(CP_ACP, 0, DXGIAdapterDesc.Description, -1, ch, 180, &DefChar, NULL);
		std::string adapterDesc(ch);
		
		// Release memory
		DXGIDevice->Release();
		DXGIAdapter->Release();

		// Log info // #todo: Log more information
		LT_CORE_INFO("dxGraphicsContext:");
		LT_CORE_INFO("        Renderer: {}", adapterDesc);
	}

	dxGraphicsContext::~dxGraphicsContext()
	{
		HRESULT hr;
		DXC(m_SwapChain->SetFullscreenState(false, nullptr));
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
		viewPort.Width  = static_cast<float>(event.GetWidth());
		viewPort.Height = static_cast<float>(event.GetHeight());
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = 0.0f;
		viewPort.TopLeftY = 0.0f;
		m_DeviceContext->RSSetViewports(1u, &viewPort);

		return false;
	}

	bool dxGraphicsContext::OnWindowMaximize(WindowMaximizedEvent& event)
	{
		HRESULT hr;
		DXC(m_SwapChain->SetFullscreenState(true, nullptr));
		return false;
	}

	bool dxGraphicsContext::OnWindowMinimize(WindowMinimizedEvent& event)
	{
		HRESULT hr;
		DXC(m_SwapChain->SetFullscreenState(false, nullptr));
		return false;
	}

	bool dxGraphicsContext::OnWindowRestore(WindowRestoredEvent& event)
	{
		HRESULT hr;
		DXC(m_SwapChain->SetFullscreenState(false, nullptr));
		return false;
	}

}