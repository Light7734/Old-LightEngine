#include "ltpch.h"
#include "Blender.h"

#include "GraphicsContext.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxBlender.h"
#endif
#include "Platform/Opengl/glBlender.h"

namespace Light {

	std::unique_ptr<Blender> Blender::s_Context;

	void Blender::Init()
	{
		LT_PROFILE_FUNC();
		s_Context.reset();

		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			s_Context = std::make_unique<glBlender>();
			break;

		case GraphicsAPI::Directx: LT_DX(
			s_Context = std::make_unique<dxBlender>();
			break;
		)
		default:
			LT_CORE_ASSERT(false, "Blender::Init: invalid GraphicsAPI");
		}
	}

	void Blender::Terminate()
	{
		LT_PROFILE_FUNC();
		s_Context.reset();
	}

}