#include "ltpch.h"
#include "dxGraphicsContext.h"
	
#include "Core/Monitor.h"
#include "Core/Window.h"

#include "Debug/Exceptions.h"

namespace Light {

	dxGraphicsContext* dxGraphicsContext::s_Instance = nullptr;

	dxGraphicsContext::dxGraphicsContext(const GraphicsConfigurations& configurations)
	{
		HRESULT hr;
		s_Instance = this;
		s_Configurations = configurations;

		// create swap chain's descriptor
		DXGI_SWAP_CHAIN_DESC sd = { 0 };

		sd.OutputWindow = static_cast<HWND>(Window::GetNativeHandle());

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1u;

		sd.BufferDesc.Width  = s_Configurations.resolution.width;
		sd.BufferDesc.Height = s_Configurations.resolution.height;
		sd.BufferDesc.RefreshRate.Denominator = NULL;
		sd.BufferDesc.RefreshRate.Numerator   = NULL;
		sd.BufferDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		sd.SampleDesc.Count   = 1u;
		sd.SampleDesc.Quality = 0u;

		sd.Windowed = true;
		sd.Flags    = NULL;

		UINT flags = NULL;
#ifdef LIGHT_DEBUG
		flags = D3D11_CREATE_DEVICE_DEBUG;
#endif
		// create device and swap chain
		DXC( D3D11CreateDeviceAndSwapChain
		(   nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flags,
			nullptr,
			NULL,
			D3D11_SDK_VERSION,
			&sd,
			&m_SwapChain,
			&m_Device,
			nullptr,
			&m_DeviceContext   ));

		if (flags == D3D11_CREATE_DEVICE_DEBUG)
		{
			Microsoft::WRL::ComPtr<ID3D11Debug> debugInterface;
			Microsoft::WRL::ComPtr<ID3D11InfoQueue> infoQueue;

			DXC(m_Device.As(&debugInterface));
			DXC(debugInterface.As(&infoQueue));

			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET,
				// #todo: add more message IDs here as needed 
			};

			D3D11_INFO_QUEUE_FILTER filter;
			memset(&filter, 0, sizeof(filter));
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			infoQueue->AddStorageFilterEntries(&filter);
		}

		// set topology
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// set graphics configurations ( resolution, vsync, etc... )
		SetConfigurations(configurations);

		// if we are in fullscreen mode, we go windowed and back to fullscreen
		if (Window::GetDisplayMode() == DisplayMode::Fullscreen)
		{
			Window::SetDisplayMode(DisplayMode::Windowed);
			Window::SetDisplayMode(DisplayMode::Fullscreen);
		}

		// log some information about dx context //
		// locals 
		IDXGIDevice*      DXGIDevice;
		IDXGIAdapter*     DXGIAdapter;
		DXGI_ADAPTER_DESC DXGIAdapterDesc;

		// initialize Locals
		DXC(m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&DXGIDevice));
		DXC(DXGIDevice->GetAdapter(&DXGIAdapter));
		DXC(DXGIAdapter->GetDesc(&DXGIAdapterDesc));

		// get the adapter's description
		char DefChar = ' ';
		char ch[180];
		WideCharToMultiByte(CP_ACP, 0, DXGIAdapterDesc.Description, -1, ch, 180, &DefChar, NULL);
		std::string adapterDesc(ch);
		
		// release memory
		DXGIDevice->Release();
		DXGIAdapter->Release();

		// log info // #todo: log more information
		LT_CORE_INFO("dxGraphicsContext:");
		LT_CORE_INFO("        Renderer: {}", adapterDesc);
	}

	void dxGraphicsContext::SwapBuffers()
	{
		m_SwapChain->Present(s_Configurations.vSync, NULL);
	}

	void dxGraphicsContext::ClearBackbuffer(float colors[4])
	{
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), colors);
	}

	void dxGraphicsContext::Draw(unsigned int count)
	{
		m_DeviceContext->Draw(count, 0u);
	}

	void dxGraphicsContext::DrawIndexed(unsigned int count)
	{
		m_DeviceContext->DrawIndexed(count, 0u, 0u);
	}


	void dxGraphicsContext::SetConfigurations(const GraphicsConfigurations& configurations)
	{
		SetResolution(configurations.resolution);
		SetVSync(configurations.vSync);
	}

	void dxGraphicsContext::SetResolution(const Resolution& resolution)
	{
		std::shared_ptr<Monitor> windowMonitor = Monitor::GetWindowMonitor();
		std::vector<VideoMode> videoModes = windowMonitor->GetVideoModes();

		int maxWidth = 0, maxHeight = 0;
		for (const auto& videoMode : videoModes)
		{
			if (videoMode.width > maxWidth)
				maxWidth = videoMode.width;

			if (videoMode.height > maxHeight)
				maxHeight = videoMode.height;
		}

		if (resolution.width > maxWidth || resolution.height > maxHeight)
		{
			LT_CORE_ERROR("GraphicsContext::SetResolution: Window's resolution cannot be higher than the Monitor's: [{}x{}] > [{}x{}]",
			              resolution.width, resolution.height,
			              maxWidth , maxHeight);
			return;
		}

		s_Configurations.resolution = resolution;

		glfwSetWindowSize(Window::GetGlfwHandle(), resolution.width, resolution.height);
		Window::Center();

		HRESULT hr;

		// remove render target
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		m_DeviceContext->OMSetRenderTargets(1, nullViews, nullptr);
		m_RenderTargetView.Reset();

		// flush
		m_DeviceContext->Flush();

		// resize buffer
		DXC(m_SwapChain->ResizeBuffers(0, resolution.width, resolution.height,
		                               DXGI_FORMAT_R8G8B8A8_UNORM, NULL));

		// create render target
		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer = nullptr;
		DXC(m_SwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &backBuffer));
		DXC(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_RenderTargetView));

		// set new render target
		m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);

		// create and set new viewport
		D3D11_VIEWPORT viewPort;
		viewPort.Width = static_cast<float>(resolution.width);
		viewPort.Height = static_cast<float>(resolution.height);
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = 0.0f;
		viewPort.TopLeftY = 0.0f;


		m_DeviceContext->RSSetViewports(1u, &viewPort);
	}

	void dxGraphicsContext::SetVSync(bool vSync)
	{
		s_Configurations.vSync = vSync;
	}

	void dxGraphicsContext::DefaultRenderBuffer()
	{
		s_Instance->m_DeviceContext->OMSetRenderTargets(1, s_Instance->m_RenderTargetView.GetAddressOf(), nullptr); 
	}

}