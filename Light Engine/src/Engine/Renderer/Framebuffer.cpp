#include "ltpch.h"
#include "Framebuffer.h"

#include "GraphicsContext.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxFramebuffer.h"
#endif
#include "Platform/Opengl/glFramebuffer.h"

namespace Light {

	std::shared_ptr<Framebuffer> Framebuffer::Create(const std::string& vertex, const std::string& fragment, TextureBindingSlot slot)
	{
		LT_PROFILE_FUNC();

		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			return std::make_shared<glFramebuffer>(vertex, fragment, GraphicsContext::GetResolution().width, GraphicsContext::GetResolution().height, slot);
		case GraphicsAPI::Directx: LT_DX(
			return std::make_shared<dxFramebuffer>(vertex, fragment, GraphicsContext::GetResolution().width, GraphicsContext::GetResolution().height, slot);)
		default:
			LT_CORE_ASSERT(false, "Framebuffer::Create: invalid GraphicsAPI");
		}
	}
			
}