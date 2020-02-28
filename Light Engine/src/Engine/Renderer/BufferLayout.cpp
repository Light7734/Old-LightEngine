#include "ltpch.h"
#include "BufferLayout.h"

#include "GraphicsContext.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxBufferLayout.h"
#endif
#include "Platform/Opengl/glBufferLayout.h"

namespace Light {
		
	std::shared_ptr<Light::BufferLayout> BufferLayout::Create(std::shared_ptr<Shader> shader,
	                                                          std::shared_ptr<VertexBuffer> buffer,
	                                                          std::shared_ptr<VertexLayout> layout)
	{
		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			return std::make_shared<glBufferLayout>(buffer, layout);
		case GraphicsAPI::Directx: LT_DX(
			return std::make_shared<dxBufferLayout>(shader, layout); )
		default:
			LT_CORE_ASSERT(false, "BufferLayout::Create: Invalid GraphicsAPI");
		}
	}

}