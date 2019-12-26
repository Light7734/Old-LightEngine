#include "ltpch.h"
#include "GraphicsContext.h"
#include "RenderCommand.h"

#include "Platform/DirectX/dxGraphicsContext.h"
#include "Platform/Opengl/glGraphicsContext.h"

namespace Light {

	GraphicsAPI GraphicsContext::s_Api;

	void GraphicsContext::Init(std::shared_ptr<Window> game_window, bool v_sync)
	{
		GraphicsContext* context;

		switch (s_Api)
		{
		case GraphicsAPI::None:
			// If GraphicsAPI is none we should choose a default GraphicsAPI
			LT_CORE_ASSERT(false, EC_FEATURE_NOT_SUPPORTED, "GraphicsAPI::None is not supported");
			break;
		case GraphicsAPI::DirectX:
			context = new dxGraphicsContext(game_window, v_sync);
			break;
		case GraphicsAPI::Opengl:
			context = new glGraphicsContext(game_window, v_sync);
			break;
		default:
			LT_CORE_ASSERT(false, EC_INVALID_GRAPHICS_API, "Invalid GraphicsAPI");
		}

		RenderCommand::SetGraphicsContext(context);
	}

}