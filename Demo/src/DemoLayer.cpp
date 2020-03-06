#include "DemoLayer.h"
#include "QuadsLayer.h"

DemoLayer::DemoLayer()
{
	m_LayeDebugrName = "DemoLayer";
	m_QuadsLayer = new QuadsLayer;
}

void DemoLayer::OnAttach()
{
	LT_TRACE("Attached DemoLayer");

	LT_INFO(" __________________________________");
	LT_INFO("|    KEY    |        ACTION        |");
	LT_INFO("|-----------|----------------------|");
	LT_INFO("|    ESC    |         Exit         |");
	LT_INFO("|     1     |  BorderlessWindowed  |");
	LT_INFO("|     2     |       Windowed       |");
	LT_INFO("|     3     |      Fullscreen      |");
	LT_INFO("|-----------|----------------------|");
	LT_INFO("|     W     |     Camera: Up       |");
	LT_INFO("|     A     |     Camera: Left     |");
	LT_INFO("|     S     |     Camera: Down     |");
	LT_INFO("|     D     |     Camera: Right    |");
	LT_INFO("|-----------|----------------------|");
	LT_INFO("|     E     |    GraphicsAPI:GL    |");
	LT_INFO("|     R     |    GraphicsAPI:DX    |");
	LT_INFO("|-----------|----------------------|");
	LT_INFO("|     Z     |  Resolution (4:3)    |");
	LT_INFO("|     X     |  Resolution (16:9)   |");
	LT_INFO("|     C     |  Resolution (16:10)  |");
	LT_INFO("|___________|______________________|");

	float colors[4] = { 0.0f, 0.0f, 0.0f, 1.0f};
	Light::RenderCommand::SetClearBackbufferColor(colors);

	Light::Blender::Enable();

	Light::LayerStack::AttachLayer(m_QuadsLayer);
}

void DemoLayer::OnDetatch()
{
	LT_TRACE("Detatched DemoLayer");
}

void DemoLayer::OnUserInterfaceUpdate()
{
	Light::UserInterface::ShowImGuiDemoWnidow();


	ImGui::Begin("DemoLayer");

	if (ImGui::TreeNode("Monitors"))
	{
		Light::Monitor::ShowDebugWindowAll();
		ImGui::TreePop();
	}
	ImGui::Separator();

	if (ImGui::TreeNode("Input"))
	{
		Light::Input::ShowDebugWindow();
		ImGui::TreePop();
	}
	ImGui::Separator();

	if (ImGui::TreeNode("Layers"))
	{
		Light::LayerStack::ShowDebugWindow();
		ImGui::TreePop();
	}
	ImGui::Separator();

	if (ImGui::TreeNode("GraphicsContext"))
	{
		Light::GraphicsContext::ShowDebugWindow();
		ImGui::TreePop();
	}
	ImGui::Separator();

	if (ImGui::TreeNode("Blender"))
	{
		Light::Blender::ShowDebugWindow();
		ImGui::TreePop();
	}

	ImGui::End();
}

void DemoLayer::OnEvent(Light::Event& event)
{
	// uncomment to log every event
	// LT_TRACE(event.GetLogInfo());

	Light::Dispatcher dispatcher(event); 
	dispatcher.Dispatch<Light::KeyboardKeyPressedEvent>(LT_EVENT_FN(DemoLayer::OnKeyPress));
}

bool DemoLayer::OnKeyPress(Light::KeyboardKeyPressedEvent& event)
{
	if (event.GetKey() == KEY_ESCAPE)
		Light::Window::Close();

	if (event.GetKey() == KEY_E || event.GetKey() == KEY_R)
	{
		Light::Window::GfxSetApi(event.GetKey() == KEY_E ? Light::GraphicsAPI::Opengl : Light::GraphicsAPI::Directx,
		                         Light::GraphicsContext::GetConfigurations());

		Light::LayerStack::DetatchLayer(m_QuadsLayer);
		Light::LayerStack::AttachLayer(m_QuadsLayer);
	}

	if (event.GetKey() == KEY_Z)
	{
		static int index = 0;
		static const Light::Resolution res[] =
		{
			{ 640 , 480 , Light::AspectRatio::AR_4_3 },
			{ 800 , 600 , Light::AspectRatio::AR_4_3 },
			{ 960 , 720 , Light::AspectRatio::AR_4_3 },
			{ 1400, 1050, Light::AspectRatio::AR_4_3 },
		};

		Light::Window::GfxSetResolution(res[++index % std::size(res)]);
	}

	if (event.GetKey() == KEY_X)
	{
		static int index = 0;
		static const Light::Resolution res[] =
		{
			{ 1280, 720  , Light::AspectRatio::AR_16_9 },
			{ 1600, 900  , Light::AspectRatio::AR_16_9 },
			{ 1920, 1080 , Light::AspectRatio::AR_16_9 },
		};

		Light::Window::GfxSetResolution(res[++index % std::size(res)]);
	}

	if (event.GetKey() == KEY_C)
	{
		static int index = 0;
		static const Light::Resolution res[] =
		{
			{ 1280, 800  ,Light::AspectRatio::AR_16_10 },
			{ 1440, 900  ,Light::AspectRatio::AR_16_10 },
			{ 1920, 1200 ,Light::AspectRatio::AR_16_10 },
			{ 1680, 1050 ,Light::AspectRatio::AR_16_10 },
		};

		Light::Window::GfxSetResolution(res[++index % std::size(res)]);
	}

	if (event.GetKey() == KEY_1)
		Light::Window::SetDisplayMode(Light::DisplayMode::BorderlessWindowed);

	if (event.GetKey() == KEY_2)
		Light::Window::SetDisplayMode(Light::DisplayMode::Windowed);

	if (event.GetKey() == KEY_3)
		Light::Window::SetDisplayMode(Light::DisplayMode::Fullscreen);

	return true;
}