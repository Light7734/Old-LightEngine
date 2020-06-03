#include "ltpch.h"
#include "UserInterface.h"

#include "Renderer/GraphicsContext.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxUserInterface.h"
#endif
#include "Platform/Opengl/glUserInterface.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>

namespace Light {

	std::unique_ptr<UserInterface> UserInterface::s_Context;

	void UserInterface::Init()
	{
		LT_PROFILE_FUNC();

		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			s_Context = std::make_unique<glUserInterface>();
			break;

		case GraphicsAPI::Directx: LT_DX(
			s_Context = std::make_unique<dxUserInterface>();
			break;
		)

		default:
			LT_CORE_ASSERT(false, "UserInterface::Init: invalid GraphicsAPI");
		}
	}

	void UserInterface::Terminate()
	{
		LT_PROFILE_FUNC();
		s_Context.reset();
	}

	void UserInterface::ShowImGuiDemoWnidow()
	{
		LT_PROFILE_FUNC();

		static bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);
	}

}