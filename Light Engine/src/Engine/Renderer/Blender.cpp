#include "ltpch.h"
#include "Blender.h"

#include "GraphicsContext.h"

#include "Platform/Opengl/glBlender.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxBlender.h"
#endif

#include <imgui.h>

namespace Light {

	Blender* Blender::s_Context = nullptr;

	bool Blender::b_Enabled = false;

	void Blender::Init()
	{
		if (s_Context)
			delete s_Context;

		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			s_Context = new glBlender;
			break;

		case GraphicsAPI::Directx:
			s_Context = new dxBlender;
			break;

		default:
			LT_CORE_ASSERT(false, "Blender::Init: Invalid GraphicsAPI");
		}

		if (b_Enabled)
			s_Context->Enable();
	}

	void Blender::ShowDebugWindow()
	{
		ImGui::Text("Blending: %s", b_Enabled ? "on" : "off");
		if (ImGui::Button("enable"))
			Enable();
		ImGui::SameLine();
		if (ImGui::Button("disable"))
			Disable();
	}

}