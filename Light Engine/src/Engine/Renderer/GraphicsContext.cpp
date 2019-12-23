#include "ltpch.h"
#include "GraphicsContext.h"
#include "RenderCommand.h"

#include "Platform/DirectX/GraphicsContext.h"

namespace Light {

	GraphicsAPI GraphicsContext::s_Api;

	void GraphicsContext::Init(std::shared_ptr<Window> window)
	{
		GraphicsContext* context;

		switch (s_Api)
		{
		case GraphicsAPI::None:
			// If GraphicsAPI is none we should choose a default GraphicsAPI
			LT_CORE_ASSERT(false, EC_FEATURE_NOT_SUPPORTED, "GraphicsAPI::None is not supported");
			break;
		case GraphicsAPI::DirectX:
			context = new dxGraphicsContext(window, window->GetWindowStyle());
			break;
		case GraphicsAPI::Opengl:
			LT_CORE_ASSERT(false, EC_FEATURE_NOT_SUPPORTED, "GraphicsAPI::OpenGL is not supported");
			break;
		default:
			LT_CORE_ASSERT(false, EC_INVALID_GRAPHICS_API, "Invalid GraphicsAPI");
		}

		RenderCommand::SetGraphicsContext(context);
	}

}