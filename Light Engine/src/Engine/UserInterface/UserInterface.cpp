#include "ltpch.h"
#include "UserInterface.h"

#include "Core/Window.h"

#include "Platform/Opengl/glUserInterface.h"
#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxUserInterface.h"
#endif

#include <imgui.h>
#include <imgui_impl_glfw.h>

namespace Light {

	UserInterface* UserInterface::s_Context = nullptr;

	void UserInterface::Init()
	{
		if (s_Context)
		{
			s_Context->TerminateImpl();
			delete s_Context;
		}

		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			s_Context = new glUserInterface;
			break;

		case GraphicsAPI::Directx: LT_DX(
			s_Context = new dxUserInterface;
			break;
		)

		default:
			LT_CORE_ASSERT(false, "UserInterface::Init: Invalid GraphicsAPI");
		}
	}

	void UserInterface::ShowImGuiDemoWnidow()
	{
		static bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);
	}

}