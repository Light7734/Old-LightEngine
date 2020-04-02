#include "ltpch.h"
#include "Framebuffer.h"

#include "GraphicsContext.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxFramebuffer.h"
#endif
#include "Platform/Opengl/glFramebuffer.h"

namespace Light {

	std::shared_ptr<Framebuffer> Framebuffer::Create(const std::string& vertex, const std::string& fragment)
	{
		LT_PROFILE_FUNC();

		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			return std::make_shared<glFramebuffer>(vertex, fragment);
		case GraphicsAPI::Directx: LT_DX(
			return std::make_shared<dxFramebuffer>(vertex, fragment);)	
		default:
			LT_CORE_ASSERT(false, "Framebuffer::Create: invalid GraphicsAPI");
		}
	}
			
}