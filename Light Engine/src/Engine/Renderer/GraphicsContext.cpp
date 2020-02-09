#include "ltpch.h"
#include "GraphicsContext.h"

#include "Camera.h"
#include "RenderCommand.h"
#include "Renderer.h"

#include "Core/Window.h"

#include "Texture.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxGraphicsContext.h"
#endif
#include "Platform/Opengl/glGraphicsContext.h"

namespace Light {

	GraphicsAPI GraphicsContext::s_Api = GraphicsAPI::Default;

	void GraphicsContext::CreateContext(GraphicsAPI api, const GraphicsConfigurations& configurations)
	{
		LT_CORE_ASSERT(Window::IsInitialized(), "Window is not initialized");

		if (s_Api == api && s_Api != GraphicsAPI::Default)
			{ LT_CORE_ERROR("Cannot recreate context with same api"); return; }

		if (api == GraphicsAPI::Default)
		{
#ifdef LIGHT_PLATFORM_WINDOWS
			s_Api = GraphicsAPI::Directx;
#else
			s_Api = GraphicsAPI::Opengl;
#endif
		}
		else
			s_Api = api;
		

		switch (s_Api)
		{
		case GraphicsAPI::Opengl:
			RenderCommand::SetGraphicsContext(std::make_unique<glGraphicsContext>(configurations));
			break;
		case GraphicsAPI::Directx: LT_DX(
			RenderCommand::SetGraphicsContext(std::make_unique<dxGraphicsContext>(configurations));
			break; )
		default:
			LT_CORE_ASSERT(false, "Invalid GraphicsAPI");
		}

		Renderer::Init();
		ConstantBuffers::Init();
		Camera::UpdateConstants();

		TextureAtlas::DestroyTextureArray();
	}

}