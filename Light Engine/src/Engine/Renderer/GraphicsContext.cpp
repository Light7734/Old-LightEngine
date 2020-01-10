#include "ltpch.h"
#include "GraphicsContext.h"

#include "Renderer.h"
#include "RenderCommand.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxGraphicsContext.h"
#endif
#include "Platform/Opengl/glGraphicsContext.h"

namespace Light {

	GraphicsAPI GraphicsContext::s_Api = GraphicsAPI::Default;
	GraphicsConfigurations GraphicsContext::s_Configurations;

	void GraphicsContext::Init(GraphicsAPI api, GraphicsConfigurations data, std::shared_ptr<Window> game_window)
	{
		std::unique_ptr<GraphicsContext> context;
		s_Api = api;

		switch (s_Api)
		{
		case GraphicsAPI::Default: 
			LT_DX // If we are on Windows, default graphics api is DirectX
			(
				RenderCommand::SetGraphicsContext(std::make_unique<dxGraphicsContext>(game_window, data));
				s_Api = GraphicsAPI::DirectX;
				break;
			) // Otherwise it's OpenGL
			RenderCommand::SetGraphicsContext(std::make_unique<glGraphicsContext>(game_window, data));
			s_Api = GraphicsAPI::Opengl;
			break;
		case GraphicsAPI::Opengl:
			RenderCommand::SetGraphicsContext(std::make_unique<glGraphicsContext>(game_window, data));
			break;
		case GraphicsAPI::DirectX: LT_DX(
			RenderCommand::SetGraphicsContext(std::make_unique<dxGraphicsContext>(game_window, data));
			break; )
		default:
			LT_CORE_ASSERT(false, EC_INVALID_GRAPHICS_API, "Invalid GraphicsAPI");
		}

		Renderer::Init();
	}

}