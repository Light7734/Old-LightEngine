#include "ltpch.h"
#include "GraphicsContext.h"

#include "Camera.h"
#include "RenderCommand.h"
#include "Renderer.h"

#include "Core/Window.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxGraphicsContext.h"
#endif
#include "Platform/Opengl/glGraphicsContext.h"

namespace Light {

	GraphicsAPI GraphicsContext::s_Api = GraphicsAPI::Default;

	void GraphicsContext::CreateContext(GraphicsAPI api, const GraphicsConfigurations& configurations)
	{
		LT_CORE_ASSERT(Window::IsInitialized(), "Window is not initialized");

		if (api == GraphicsAPI::Default)
		{
#ifdef LIGHT_PLATFORM_WINDOWS
			s_Api = GraphicsAPI::DirectX;
#else
			s_Api = GraphicsAPI::Opengl;
#endif
		}
		else
			s_Api = api;
		

		RenderCommand::SetGraphicsContext(nullptr); // To make the gl/dx GraphicsContext dtor get called before ctor
		switch (s_Api)
		{
		case GraphicsAPI::Opengl:
			RenderCommand::SetGraphicsContext(std::make_unique<glGraphicsContext>(configurations));
			break;
		case GraphicsAPI::DirectX: LT_DX(
			RenderCommand::SetGraphicsContext(std::make_unique<dxGraphicsContext>(configurations));
			break; )
		default:
			LT_CORE_ASSERT(false, "Invalid GraphicsAPI");
		}

		Renderer::Init();
		ConstantBuffers::Init();
		Camera::UpdateConstants();
	}

}