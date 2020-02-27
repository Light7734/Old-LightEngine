#include "ltpch.h"
#include "GraphicsContext.h"

#include "Camera.h"
#include "RenderCommand.h"
#include "Renderer.h"
#include "Texture.h"

#include "Core/Window.h"

#include "UserInterface/UserInterface.h"

#ifdef LIGHT_PLATFORM_WINDOWS
	#include "Platform/DirectX/dxGraphicsContext.h"
#endif
#include "Platform/Opengl/glGraphicsContext.h"


#include <glfw/glfw3.h>

#include <imgui.h>

namespace Light {

	GraphicsProperties GraphicsContext::s_Properties;
	GraphicsConfigurations GraphicsContext::s_Configurations;

	GraphicsAPI GraphicsContext::s_Api = GraphicsAPI::Default;

	std::unique_ptr<GraphicsContext> GraphicsContext::Create(GraphicsAPI api, const GraphicsConfigurations& configurations)
	{
		if(s_Api == GraphicsAPI::Default) // First call
		{
			s_Properties.primaryMonitor = Monitor::GetMonitor(0);

			const GLFWvidmode* mode = s_Properties.primaryMonitor->GetVideoMode();
			float ratio = (float)mode->width / mode->height;
			s_Properties.primaryMonitorRes = Resolution(mode->width, mode->height,
			                                            ratio == 4.0f / 3.0f ? AspectRatio::AR_4_3     :
			                                            ratio == 16.0f / 9.0f ? AspectRatio::AR_16_9   :
			                                            ratio == 16.0f / 10.0f ? AspectRatio::AR_16_10 :
			                                            AspectRatio());
		}
		else
			TextureAtlas::DestroyTextureArray(); // Destroy previous GraphicsAPI's texture array


		if (api == GraphicsAPI::Default)
		{
#ifdef LIGHT_PLATFORM_WINDOWS
			s_Api = GraphicsAPI::Directx;
#else
			s_Api = GraphicsAPI::Opengl;
#endif
		}
		else
			s_Api = api;
		

		switch (s_Api)
		{
		case GraphicsAPI::Opengl:
		{
			std::unique_ptr<glGraphicsContext> context = std::make_unique<glGraphicsContext>(configurations);

			RenderCommand::SetGraphicsContext(context.get());
			Renderer::Init();
			ConstantBuffers::Init();
			UserInterface::Init();

			return std::move(context);
		}
		case GraphicsAPI::Directx: LT_DX(
		{
			std::unique_ptr<dxGraphicsContext> context = std::make_unique<dxGraphicsContext>(configurations);

			RenderCommand::SetGraphicsContext(context.get());
			Renderer::Init();
			ConstantBuffers::Init();
			UserInterface::Init();

			return std::move(context);
		})
		default:
			LT_CORE_ASSERT(false, "GraphicsContext::Create: Invalid GraphicsAPI");
		}
	}

	void GraphicsContext::ShowDebugWindow()
	{
		ImGui::Begin("Light::GraphicsContext");

		ImGui::Text("graphics api: %s", s_Api == GraphicsAPI::Opengl  ? "opengl"  :
		                                s_Api == GraphicsAPI::Directx ? "directx" : "");
		if (ImGui::TreeNode("configurations"))
		{
			ImGui::Text("resolution:");
			ImGui::BulletText("size: [%d x %d]", s_Configurations.resolution.width, s_Configurations.resolution.height);
			ImGui::BulletText("aspect ratio: %f", s_Configurations.resolution.aspectRatio);
			ImGui::BulletText("v-sync: %s", s_Configurations.vSync ? "on" : "off");

			ImGui::TreePop();
		}
		// #todo: properties...

		ImGui::End();
	}

}