#include "ltpch.h"
#include "GraphicsContext.h"
#include "RenderCommand.h"

#include "Platform/DirectX/dxGraphicsContext.h"
#include "Platform/Opengl/glGraphicsContext.h"

namespace Light {

	GraphicsAPI GraphicsContext::s_Api = GraphicsAPI::Default;
	GraphicsConfigurations GraphicsContext::s_Configurations;

	void GraphicsContext::Init(GraphicsAPI api, GraphicsConfigurations data, std::shared_ptr<Window> game_window)
	{
		std::unique_ptr<GraphicsContext> context;
		s_Api = api;

		// #todo remove directX stuff if we are not in windows
		switch (s_Api)
		{
		case GraphicsAPI::Default:
			RenderCommand::SetGraphicsContext(std::make_unique<dxGraphicsContext>(game_window, data));
			s_Api = GraphicsAPI::DirectX;
			break;
		case GraphicsAPI::DirectX: 
			RenderCommand::SetGraphicsContext(std::make_unique<dxGraphicsContext>(game_window, data));
			break;
		case GraphicsAPI::Opengl:
			RenderCommand::SetGraphicsContext(std::make_unique<glGraphicsContext>(game_window, data));
			break;
		default:
			LT_CORE_ASSERT(false, EC_INVALID_GRAPHICS_API, "Invalid GraphicsAPI");
		}

	}

}