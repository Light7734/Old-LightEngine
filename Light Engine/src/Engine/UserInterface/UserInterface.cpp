#include "ltpch.h"
#include "UserInterface.h"

#include "Core/Window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>

#include "Platform/Opengl/glUserInterface.h"
#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxUserInterface.h"
#endif

#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"

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
			LT_CORE_ASSERT(FALSE, "UserInterface::Init: Invalid GraphicsAPI");
		}
	}

	void UserInterface::ShowImGuiDemoWnidow()
	{
		static bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	void UserInterface::OnEvent(Event& event)
	{
		Dispatcher dispatcher(event);

		dispatcher.Dispatch<KeyboardKeyPressedEvent>(LT_EVENT_FN_STATIC(UserInterface::OnKeyPress));
		dispatcher.Dispatch<KeyboardKeyReleasedEvent>(LT_EVENT_FN_STATIC(UserInterface::OnKeyRelease));

		dispatcher.Dispatch<MouseMovedEvent>(LT_EVENT_FN_STATIC(UserInterface::OnMouseMove));
		dispatcher.Dispatch<MouseButtonPressedEvent>(LT_EVENT_FN_STATIC(UserInterface::OnButtonPress));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(LT_EVENT_FN_STATIC(UserInterface::OnButtonRelease));
		dispatcher.Dispatch<MouseScrolledEvent>(LT_EVENT_FN_STATIC(UserInterface::OnMouseScroll));
	}

	bool UserInterface::OnKeyPress(KeyboardKeyPressedEvent& event)
	{
		ImGui::GetIO().KeysDown[event.GetKey()] = true;
		return false;
	}

	bool UserInterface::OnKeyRelease(KeyboardKeyReleasedEvent& event)
	{
		ImGui::GetIO().KeysDown[event.GetKey()] = false;
		return false;
	}

	bool UserInterface::OnMouseMove(MouseMovedEvent& event)
	{
		ImGui::GetIO().MousePos = ImVec2(event.GetX(), event.GetY());
		return false;
	}

	bool UserInterface::OnButtonPress(MouseButtonPressedEvent& event)
	{
		ImGui::GetIO().MouseDown[event.GetButton()] = true;
		return false;
	}

	bool UserInterface::OnButtonRelease(MouseButtonReleasedEvent& event)
	{
		ImGui::GetIO().MouseDown[event.GetButton()] = false;
		return false;
	}

	bool UserInterface::OnMouseScroll(MouseScrolledEvent& event)
	{
		ImGui::GetIO().MouseWheel = event.GetOffset();
		return false;
	}

}