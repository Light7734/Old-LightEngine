#include "ltpch.h"
#include "glUserInterface.h"

#include "Core/Window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Light {

	glUserInterface::glUserInterface()
	{
		// setup ImGui's context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // enable keyboard controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // enable gamepad controls

		// setup ImGui's style
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;

		// setup platform/renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(Window::GetGlfwHandle(), false);
		ImGui_ImplOpenGL3_Init("#version 450 core");
	}

	glUserInterface::~glUserInterface()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void glUserInterface::BeginImpl()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void glUserInterface::EndImpl()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}