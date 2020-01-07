#include "ltpch.h"
#include "Buffers.h"

#include "GraphicsContext.h"

#include "Platform/DirectX/dxBuffers.h"
#include "Platform/Opengl/glBuffers.h"

namespace Light {

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, unsigned int size, unsigned int stride)
	{
		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Default:
			LT_CORE_ASSERT(false, EC_NO_INIT_GRAPHICSC_CONTEXT, "Failed to create VertexBuffer: GraphicsContext::Init was never called");
		case GraphicsAPI::DirectX:
			return std::make_shared<dxVertexBuffer>(vertices, size, stride);
		case GraphicsAPI::Opengl:
			return std::make_shared<glVertexBuffer>(vertices, size);
		default:
			LT_CORE_ASSERT(false, EC_INVALID_GRAPHICS_API, "Invalid GraphicsAPI");
		}
	}

}