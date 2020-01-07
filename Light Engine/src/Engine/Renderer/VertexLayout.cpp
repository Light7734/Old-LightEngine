#include "ltpch.h"
#include "VertexLayout.h"

#include "GraphicsContext.h"

#include "Platform/DirectX/dxVertexLayout.h"
#include "Platform/Opengl/glVertexLayout.h"

namespace Light {

	std::shared_ptr<Light::VertexLayout> VertexLayout::Create(std::initializer_list<std::pair<const char*, VertexType>> init_list)
	{
		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Default:
			LT_CORE_ASSERT(false, EC_NO_INIT_GRAPHICSC_CONTEXT, "Failed to create VertexLayout: GraphicsContext::Init was never called");
		case GraphicsAPI::DirectX:
			return std::make_shared<dxVertexLayout>(init_list);
		case GraphicsAPI::Opengl:
			return std::make_shared<glVertexLayout>(init_list);
		default:
			LT_CORE_ASSERT(false, EC_INVALID_GRAPHICS_API, "Invalid GraphicsAPI");

		}
	}

}