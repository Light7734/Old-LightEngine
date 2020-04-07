#include "DemoLayer.h"

#include "AudioLayer.h"
#include "QuadsLayer.h"
#include "PostProcessLayer.h"
#include "TextLayer.h"

DemoLayer::DemoLayer()
	: m_CameraSpeed(525)
{
	LT_PROFILE_FUNC();

	m_LayeDebugrName = "DemoLayer";

	m_Camera = std::make_shared<Light::Camera>(glm::vec2(500.0f, 500.0f), Light::GraphicsContext::GetAspectRatio(), 1000.0f);

	m_AudioLayer = new AudioLayer;
	m_QuadsLayer = new QuadsLayer(m_Camera);
	m_PostProcessLayer = new PostProcessLayer;
	m_TextLayer = new TextLayer;
}

void DemoLayer::OnAttach()
{
	LT_TRACE("Attached: {}", m_LayeDebugrName);

	Light::Renderer::SetCamera(m_Camera);

	Light::Window::Get()->SetMouseCursor("res/cursor.png", 1u, 1u);

	Light::Application::AttachLayer(m_AudioLayer);
	Light::Application::AttachLayer(m_QuadsLayer);
	Light::Application::AttachLayer(m_PostProcessLayer);
	Light::Application::AttachLayer(m_TextLayer);
}

void DemoLayer::OnDetatch()
{
	LT_TRACE("Detached: {}", m_LayeDebugrName);
}

void DemoLayer::OnUpdate(float DeltaTime)
{
	if (Light::Input::GetKey(KEY_A))
		m_Camera->MoveX(-m_CameraSpeed * DeltaTime);
	if (Light::Input::GetKey(KEY_D))
		m_Camera->MoveX(m_CameraSpeed * DeltaTime);

	if (Light::Input::GetKey(KEY_W))
		m_Camera->MoveY(-m_CameraSpeed * DeltaTime);
	if (Light::Input::GetKey(KEY_S))
		m_Camera->MoveY(m_CameraSpeed * DeltaTime);
}

void DemoLayer::OnUserInterfaceUpdate()
{
	ImGui::Begin("DemoLayer");

	if (ImGui::TreeNode("Keyboard map"))
	{
		ImGui::Text(" __________________________________");
		ImGui::Text("|    KEY    |        ACTION        |");
		ImGui::Text("|-----------|----------------------|");
		ImGui::Text("|    ESC    |         Exit         |");
		ImGui::Text("|     1     |  BorderlessWindowed  |");
		ImGui::Text("|     2     |       Windowed       |");
		ImGui::Text("|     3     |      Fullscreen      |");
		ImGui::Text("|-----------|----------------------|");
		ImGui::Text("|     W     |     Camera: Up       |");
		ImGui::Text("|     A     |     Camera: Left     |");
		ImGui::Text("|     S     |     Camera: Down     |");
		ImGui::Text("|     D     |     Camera: Right    |");
		ImGui::Text("|-----------|----------------------|");
		ImGui::Text("|     E     |    GraphicsAPI:GL    |");
		ImGui::Text("|     R     |    GraphicsAPI:DX    |");
		ImGui::Text("|-----------|----------------------|");
		ImGui::Text("|     Z     |  Resolution (4:3)    |");
		ImGui::Text("|     X     |  Resolution (16:9)   |");
		ImGui::Text("|     C     |  Resolution (16:10)  |");
		ImGui::Text("|___________|______________________|");

		ImGui::TreePop();
	} 
	ImGui::Separator();

	if (ImGui::TreeNode("Mouse map"))
	{
		ImGui::Text(" _______________________________________");
		ImGui::Text("|     BUTTON    |        ACTION        |");
		ImGui::Text("|---------------|----------------------|");
		ImGui::Text("|  LEFT_BUTTON  |      Drag Sprite     |");
		ImGui::Text("|---------------|----------------------|");
		ImGui::Text("|  CTRL + WHEEL |      Zoom In/Out     |");
		ImGui::Text("|_______________|______________________|");

		ImGui::TreePop();
	}
	ImGui::Separator();

	if (ImGui::TreeNode("Monitors"))
	{
		Light::Monitor::ShowDebugWindowAll();
		ImGui::TreePop();
	}
	ImGui::Separator();

	if (ImGui::TreeNode("Layers"))
	{
		Light::Application::ShowDebugWindow();
		ImGui::TreePop();
	}
	ImGui::Separator();

	if (ImGui::TreeNode("Camera"))
	{
		m_Camera->ShowDebugWindow();
		ImGui::BulletText("speed %.2f", m_CameraSpeed);
		ImGui::TreePop();
	}
	ImGui::Separator();

	if (ImGui::TreeNode("GraphicsContext"))
	{
		Light::GraphicsContext::Get()->ShowDebugWindow();
		ImGui::TreePop();
	}
	ImGui::Separator();

	if (ImGui::TreeNode("Input"))
	{
		Light::Input::ShowDebugWindow();
		ImGui::TreePop();
	}
	ImGui::Separator();

	ImGui::End();
}

void DemoLayer::OnEvent(Light::Event& event)
{
	// uncomment to log every event
	// LT_TRACE(event.GetLogInfo());

	Light::Dispatcher dispatcher(event); 
	dispatcher.Dispatch<Light::KeyboardKeyPressedEvent>(LT_EVENT_FN(DemoLayer::OnKeyPress));
	dispatcher.Dispatch<Light::MouseScrolledEvent>(LT_EVENT_FN(DemoLayer::OnMouseScroll));

	dispatcher.Dispatch<Light::WindowResizedEvent>(LT_EVENT_FN(DemoLayer::OnWindowResize));
}

bool DemoLayer::OnKeyPress(Light::KeyboardKeyPressedEvent& event)
{
	if (event.GetKey() == KEY_ESCAPE)
		Light::Window::Get()->Close();

	if (event.GetKey() == KEY_E || event.GetKey() == KEY_R)
	{
		Light::GraphicsContext::CreateContext(event.GetKey() == KEY_E ? Light::GraphicsAPI::Opengl : Light::GraphicsAPI::Directx,
		                                      Light::GraphicsContext::GetConfigurations());

		Light::Application::DetachLayer(m_QuadsLayer);
		Light::Application::DetachLayer(m_PostProcessLayer);
		Light::Application::DetachLayer(m_TextLayer);

		delete m_QuadsLayer;
		delete m_PostProcessLayer;
		delete m_TextLayer;

		m_QuadsLayer = new QuadsLayer(m_Camera);
		m_PostProcessLayer = new PostProcessLayer;
		m_TextLayer = new TextLayer;

		Light::Application::AttachLayer(m_QuadsLayer);
		Light::Application::AttachLayer(m_PostProcessLayer);
		Light::Application::AttachLayer(m_TextLayer);
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

		Light::GraphicsContext::Get()->SetResolution(res[++index % std::size(res)]);
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

		Light::GraphicsContext::Get()->SetResolution(res[++index % std::size(res)]);
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

		Light::GraphicsContext::Get()->SetResolution(res[++index % std::size(res)]);
	}

	if (event.GetKey() == KEY_1)
		Light::Window::Get()->SetDisplayMode(Light::DisplayMode::BorderlessWindowed);

	if (event.GetKey() == KEY_2)
		Light::Window::Get()->SetDisplayMode(Light::DisplayMode::Windowed);

	if (event.GetKey() == KEY_3)
		Light::Window::Get()->SetDisplayMode(Light::DisplayMode::Fullscreen);

	return true;
}

bool DemoLayer::OnWindowResize(Light::WindowResizedEvent& event)
{
	m_Camera->SetProjection(Light::GraphicsContext::GetAspectRatio(), m_Camera->GetZoomLevel());
	return false;
}

bool DemoLayer::OnMouseScroll(Light::MouseScrolledEvent& event)
{
	if (Light::Input::GetKey(KEY_LEFT_CONTROL))
		m_Camera->Zoom(event.GetOffset() * 25);

	return false;
}