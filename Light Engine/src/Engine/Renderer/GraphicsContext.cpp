#include "ltpch.h"
#include "GraphicsContext.h"
#include "RenderCommand.h"

#include "Platform/DirectX/dxGraphicsContext.h"
#include "Platform/Opengl/glGraphicsContext.h"

namespace Light {

	GraphicsAPI GraphicsContext::s_Api = GraphicsAPI::Default;

	void GraphicsContext::Init(GraphicsAPI api, GraphicsData data, std::shared_ptr<Window> game_window)
	{
		std::unique_ptr<GraphicsContext> context;
		s_Api = api;

		// #todo remove directX stuff if we are not in windows
		switch (s_Api)
		{
		case GraphicsAPI::Default:
			context = std::make_unique<dxGraphicsContext>(game_window, data);
			break;
		case GraphicsAPI::DirectX: 
			context = std::make_unique<dxGraphicsContext>(game_window, data);
			break;
		case GraphicsAPI::Opengl:
			context = std::make_unique<glGraphicsContext>(game_window, data);
			break;
		default:
			LT_CORE_ASSERT(false, EC_INVALID_GRAPHICS_API, "Invalid GraphicsAPI");
		}

		RenderCommand::SetGraphicsContext(std::move(context));
	}

}