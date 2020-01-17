#include "ltpch.h"
#include "Buffers.h"

#include "GraphicsContext.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxBuffers.h"
#endif
#include "Platform/Opengl/glBuffers.h"

namespace Light {

	std::unique_ptr<ConstantBuffers> ConstantBuffers::s_Instance;

	void ConstantBuffers::Init()
	{
		s_Instance.reset();
		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			s_Instance = std::make_unique<glUniformBuffers>();
			break;
		case GraphicsAPI::DirectX: LT_DX(
			s_Instance = std::make_unique<dxConstantBuffers>();
			break; )
		default:
			LT_CORE_ASSERT(false, "Invalid GraphicsAPI");
		}
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, unsigned int size, unsigned int stride)
	{
		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			return std::make_shared<glVertexBuffer>(vertices, size);
		case GraphicsAPI::DirectX: LT_DX(
			return std::make_shared<dxVertexBuffer>(vertices, size, stride); )
		default:
			LT_CORE_ASSERT(false, "Invalid GraphicsAPI");
		}
	}

	std::shared_ptr<Light::IndexBuffer> IndexBuffer::Create(unsigned int* indices, unsigned int size)
	{
		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			return std::make_shared<glIndexBuffer>(indices, size);
		case GraphicsAPI::DirectX: LT_DX(
			return std::make_shared<dxIndexBuffer>(indices, size); )
		default:
			LT_CORE_ASSERT(false, "Invalid GraphicsAPI");
		}
	}

}