#include "ltpch.h"
#include "VertexLayout.h"

#include "GraphicsContext.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxVertexLayout.h"
#endif
#include "Platform/Opengl/glVertexLayout.h"

namespace Light {

	std::shared_ptr<VertexLayout> VertexLayout::Create(std::shared_ptr<Shader> shader,
	                                                   std::shared_ptr<VertexBuffer> buffer,
	                                                   std::initializer_list<std::pair<const char*, VertexElementType>> elements)
	{
		LT_PROFILE_FUNC();

		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			LT_CORE_ASSERT(buffer, "VertexLayout::Create: VertexBuffer cannot be null with opengl graphics api");
			return std::make_shared<glVertexLayout>(buffer, elements);

		case GraphicsAPI::Directx: LT_DX(
			// LT_CORE_ASSERT(shader, "VertexLayout::Create: Shader cannot be null with directx graphics api");
			return std::make_shared<dxVertexLayout>(shader, elements); )

		default:
			LT_CORE_ASSERT(false, "VertexLayout::Create: invalid GraphicsAPI");
		}
	}

}