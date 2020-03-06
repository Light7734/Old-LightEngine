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

	BlendFactor Blender::s_SrcFactor = BlendFactor::SRC_ALPHA;
	BlendFactor Blender::s_DstFactor = BlendFactor::DST_ALPHA;

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

		if (ImGui::BeginCombo("Source factor", FactorToString(s_SrcFactor)))
		{
			for (int i = 0; i < 10; i++)
				if (ImGui::Selectable(FactorToString((BlendFactor)i), false))
					SetSrcFactor((BlendFactor)i);

			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Destination Factor", FactorToString(s_DstFactor)))
		{
			for (int i = 0; i < 10; i++)
				if (ImGui::Selectable(FactorToString((BlendFactor)i), false))
					SetDstFactor((BlendFactor)i);

			ImGui::EndCombo();
		}


		ImGui::Text("%s", b_Enabled ? "enabled" : "disabled");

		if (ImGui::Button("enable"))
			Enable();
		ImGui::SameLine();
		if (ImGui::Button("disable"))
			Disable();
	}

	const char* Blender::FactorToString(BlendFactor factor)
	{
		switch (factor)
		{
		case Light::BlendFactor::ZERO:                 return "ZERO";
		case Light::BlendFactor::ONE:                  return "ONE";
		case Light::BlendFactor::SRC_COLOR:            return "SRC_COLOR";
		case Light::BlendFactor::SRC_COLOR_INVERSE:    return "SRC_COLOR_INVERSE";
		case Light::BlendFactor::SRC_ALPHA:            return "SRC_ALPHA";
		case Light::BlendFactor::SRC_ALPHA_INVERSE:    return "SRC_ALPHA_INVERSE";
		case Light::BlendFactor::DST_COLOR:            return "DST_COLOR";
		case Light::BlendFactor::DST_COLOR_INVERSE:    return "DST_COLOR_INVERSE";
		case Light::BlendFactor::DST_ALPHA:            return "DST_ALPHA";
		case Light::BlendFactor::DST_ALPHA_INVERSE:    return "DST_ALPHA_INVERSE";

		default: return "Invalid/Unknown value";
		}
	}

}