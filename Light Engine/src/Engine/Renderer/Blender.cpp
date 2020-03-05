#include "ltpch.h"
#include "Blender.h"

#include "GraphicsContext.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxBlender.h"
#endif
#include "Platform/Opengl/glBlender.h"

#include <imgui.h>

namespace Light {

	std::unique_ptr<Blender> Blender::s_Context;
	bool Blender::b_Enabled = false;

	void Blender::Init()
	{
		s_Context.reset();

		switch (GraphicsContext::GetAPI())
		{
		case GraphicsAPI::Opengl:
			s_Context = std::make_unique<glBlender>();
			break;

		case GraphicsAPI::Directx:
			s_Context = std::make_unique<dxBlender>();
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