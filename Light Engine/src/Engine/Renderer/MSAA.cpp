#include "ltpch.h"
#include "MSAA.h"

#include "GraphicsContext.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/Directx/dxMSAA.h"
#endif
#include "Platform/Opengl/glMSAA.h"

namespace Light {

	std::shared_ptr<Light::MSAA> MSAA::Create(unsigned int samples)
	{
		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			return std::make_shared<glMSAA>(samples);
		case GraphicsAPI::Directx:LT_DX(
			return std::make_shared<dxMSAA>(samples);)
		default:
			LT_CORE_ASSERT(false, "MSAA::Create: invalid GraphicsAPI");
		}
	}

}