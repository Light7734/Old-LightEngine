#include "ltpch.h"
#include "VertexLayout.h"

#include "GraphicsContext.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxVertexLayout.h"
#endif
#include "Platform/Opengl/glVertexLayout.h"

namespace Light {

	std::shared_ptr<Light::VertexLayout> VertexLayout::Create(std::initializer_list<std::pair<const char*, VertexType>> initList)
	{
		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			return std::make_shared<glVertexLayout>(initList);
		case GraphicsAPI::Directx: LT_DX(
			return std::make_shared<dxVertexLayout>(initList);)
		default:
			LT_CORE_ASSERT(false, "VertexLayout::Create: Invalid GraphicsAPI");
		}
	}

}