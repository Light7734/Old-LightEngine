#include "ltpch.h"
#include "Buffers.h"

#include "GraphicsContext.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxBuffers.h"
#endif
#include "Platform/Opengl/glBuffers.h"

namespace Light {

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, unsigned int size, unsigned int stride)
	{
		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Default:
			LT_CORE_ASSERT(false, EC_NO_INIT_GRAPHICSC_CONTEXT, "Failed to create VertexBuffer: GraphicsContext::Init was never called");
		case GraphicsAPI::Opengl:
			return std::make_shared<glVertexBuffer>(vertices, size);
		case GraphicsAPI::DirectX: LT_DX(
			return std::make_shared<dxVertexBuffer>(vertices, size, stride); )
		default:
			LT_CORE_ASSERT(false, EC_INVALID_GRAPHICS_API, "Invalid GraphicsAPI");
		}
	}

	std::shared_ptr<Light::IndexBuffer> IndexBuffer::Create(unsigned int* indices, unsigned int size)
	{
		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Default:
			LT_CORE_ASSERT(false, EC_NO_INIT_GRAPHICSC_CONTEXT, "Failed to create IndexBuffer: GraphicsContext::Init was never called");
		case GraphicsAPI::Opengl:
			return std::make_shared<glIndexBuffer>(indices, size);
		case GraphicsAPI::DirectX: LT_DX(
			return std::make_shared<dxIndexBuffer>(indices, size); )
		default:
			LT_CORE_ASSERT(false, EC_INVALID_GRAPHICS_API, "Invalid GraphicsAPI");
		}
	}

}