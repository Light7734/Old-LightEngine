#include "ltpch.h"
#include "GraphicsContext.h"

#include "Blender.h"
#include "Buffers.h"
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

	GraphicsConfigurations GraphicsContext::s_Configurations;

	GraphicsAPI GraphicsContext::s_Api = GraphicsAPI::Default;

	std::unique_ptr<GraphicsContext> GraphicsContext::Create(GraphicsAPI api, const GraphicsConfigurations& configurations)
	{
		if(s_Api != GraphicsAPI::Default)
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
			Blender::Init();
			Renderer::Init();
			ConstantBuffers::Init();
			UserInterface::Init();

			return std::move(context);
		}
		case GraphicsAPI::Directx: LT_DX(
		{
			std::unique_ptr<dxGraphicsContext> context = std::make_unique<dxGraphicsContext>(configurations);

			RenderCommand::SetGraphicsContext(context.get());
			Blender::Init();
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
		ImGui::BulletText("graphics api: %s", s_Api == GraphicsAPI::Opengl  ? "opengl"  :
		                                      s_Api == GraphicsAPI::Directx ? "directx" : "");


		ImGui::BulletText("resolution: [%d x %d]", s_Configurations.resolution.width, s_Configurations.resolution.height);
		ImGui::BulletText("aspect ratio: %f", s_Configurations.resolution.aspectRatio);
		ImGui::BulletText("v-sync: %s", s_Configurations.vSync ? "on" : "off");
		// #todo: properties...
	}

}