#include "QuadsLayer.h"

QuadsLayer::QuadsLayer()
	: m_Camera(glm::vec2(500.0f, 500.0f), Light::GraphicsContext::GetAspectRatio(), 1000.0f), m_CameraSpeed(500),
	  m_SelectedSprite(nullptr)
{
	m_LayeDebugrName = "QuadsLayer";

	srand(time(NULL));

	for (int i = 0; i < 125; i++)
	{
		Sprite sprite;

		sprite.size = glm::vec2(100.0f, 100.0f);
		sprite.position.x = std::rand() % 1000;
		sprite.position.y = std::rand() % 1000;

		m_Sprites.push_back(sprite);
	}
}

void QuadsLayer::OnAttach()
{
	LT_TRACE("Attached QuadLayer");

	m_Camera.SetProjection(Light::GraphicsContext::GetAspectRatio(), m_Camera.GetZoomLevel());

	m_TextureAtlas = Light::TextureAtlas::Create("res/atlas.txt");
	Light::TextureCoordinates* planeCoords = m_TextureAtlas->GetCoordinates("plane");

	for (int i = 0; i < m_Sprites.size(); i++)
	{
		m_Sprites[i].coordinates = planeCoords;
		m_Sprites[i].tint.r = (float)(std::rand() % 1000) / 1000.0f;
		m_Sprites[i].tint.g = (float)(std::rand() % 1000) / 1000.0f;
		m_Sprites[i].tint.b = (float)(std::rand() % 1000) / 1000.0f;
		m_Sprites[i].tint.a = 0.5f;
	}
}

void QuadsLayer::OnDetatch()
{
	LT_TRACE("Detatched QuadLayer");
}

void QuadsLayer::OnUpdate(float DeltaTime)
{
	if (m_SelectedSprite)
		m_SelectedSprite->position = Light::Input::MousePosToCameraView(m_Camera) - m_SelectedSprite->size / 2.0f;

	if (Light::Input::GetKey(KEY_A))
		m_Camera.MoveX(-m_CameraSpeed * DeltaTime);
	if (Light::Input::GetKey(KEY_D))
		m_Camera.MoveX(m_CameraSpeed * DeltaTime);

	if (Light::Input::GetKey(KEY_W))
		m_Camera.MoveY(-m_CameraSpeed * DeltaTime);
	if (Light::Input::GetKey(KEY_S))
		m_Camera.MoveY(m_CameraSpeed * DeltaTime);
}

void QuadsLayer::OnRender()
{
	Light::Renderer::Start(m_Camera);

	for (auto& sprite : m_Sprites)
		Light::Renderer::DrawQuad(sprite.position, sprite.size, sprite.coordinates, sprite.tint);

	Light::Renderer::End();
}

void QuadsLayer::ShowDebugWindow()
{
	if (ImGui::TreeNode("Camera"))
	{
		m_Camera.ShowDebugLayer();
		ImGui::BulletText("speed: %f", m_CameraSpeed);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sprites"))
	{
		ImGui::BulletText("you can drag sprites with mouse!");
		ImGui::TreePop();
	}
}

void QuadsLayer::OnEvent(Light::Event& event)
{
	Light::Dispatcher dispatcher(event);

	dispatcher.Dispatch<Light::MouseButtonPressedEvent>(LT_EVENT_FN(QuadsLayer::OnButtonPress));
	dispatcher.Dispatch<Light::MouseButtonReleasedEvent>(LT_EVENT_FN(QuadsLayer::OnButtonRelease));

	dispatcher.Dispatch<Light::MouseScrolledEvent>(LT_EVENT_FN(QuadsLayer::OnMouseScroll));

	dispatcher.Dispatch<Light::WindowResizedEvent>(LT_EVENT_FN(QuadsLayer::OnWindowResize));
}

bool QuadsLayer::OnButtonPress(Light::MouseButtonPressedEvent& event)
{
	glm::vec2 mouse = Light::Input::MousePosToCameraView(m_Camera);

	for (auto& sprite : m_Sprites)
	{
		if (mouse.x > sprite.position.x&& mouse.x < sprite.position.x + sprite.size.x &&
			mouse.y > sprite.position.y&& mouse.y < sprite.position.y + sprite.size.y)
		{
			m_SelectedSprite = &sprite;
			return true;
		}
	}
	return true;
}

bool QuadsLayer::OnButtonRelease(Light::MouseButtonReleasedEvent& event)
{
	m_SelectedSprite = nullptr;
	return true;
}

bool QuadsLayer::OnMouseScroll(Light::MouseScrolledEvent& event)
{
	if(Light::Input::GetKey(KEY_LEFT_CONTROL))
		m_Camera.Zoom(event.GetOffset() * 25);
	return true;
}

bool QuadsLayer::OnWindowResize(Light::WindowResizedEvent& event)
{
	m_Camera.SetProjection(Light::GraphicsContext::GetAspectRatio(), m_Camera.GetZoomLevel());
	return true;
}