#include "ltpch.h"
#include "dxUserInterface.h"

#include "dxGraphicsContext.h"

#include "Core/Window.h"

#include "Renderer/RenderCommand.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

namespace Light {

	dxUserInterface::dxUserInterface()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
		
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;

		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init(Window::GetNativeHandle());
		ImGui_ImplDX11_Init(dxGraphicsContext::GetDevice(), dxGraphicsContext::GetDeviceContext());
	}

	dxUserInterface::~dxUserInterface()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void dxUserInterface::BeginImpl()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void dxUserInterface::EndImpl()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

}