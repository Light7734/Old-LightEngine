#include "QuadsLayer.h"

QuadsLayer::QuadsLayer()
	: m_CameraSpeed(500), m_SelectedSprite(nullptr)
{
	m_LayeDebugrName = "QuadsLayer";

	m_Camera = std::make_shared<Light::Camera>(glm::vec2(500.0f, 500.0f), Light::GraphicsContext::GetAspectRatio(), 1000.0f);

	srand(time(NULL));

	for (int i = 0; i < 125; i++)
	{
		Sprite sprite;

		sprite.size = glm::vec2(100.0f, 100.0f);
		sprite.position.x = std::rand() % 1000;
		sprite.position.y = std::rand() % 1000;
		sprite.tint.r = 1.0f;
		sprite.tint.g = 1.0f; // (float)(std::rand() % 1000) / 1000.0f;
		sprite.tint.b = 1.0f; // (float)(std::rand() % 1000) / 1000.0f;
		sprite.tint.a = 1.0;

		m_Sprites.push_back(sprite);
	}
}

void QuadsLayer::OnAttach()
{
	Light::Renderer::SetCamera(m_Camera);

	LT_TRACE("Attached QuadLayer");

	m_Camera->SetProjection(Light::GraphicsContext::GetAspectRatio(), m_Camera->GetZoomLevel());

	m_TextureArray = Light::TextureArray::Create(1u);
	std::shared_ptr<Light::TextureAtlas> textureAtlas;

	m_TextureArray->CreateAtlas("atlas", "res/atlas.png", "res/atlas.txt");

	textureAtlas = m_TextureArray->GetAtlas("atlas");
	
	Light::TextureCoordinates* planeCoords = textureAtlas->GetCoordinates("box");

	for (int i = 0; i < m_Sprites.size(); i++)
		m_Sprites[i].coordinates = planeCoords;

	m_Grayscale = Light::Framebuffer::Create("res/FramebuffersVS.shader", "res/GrayscalePS.shader");
	m_Inverse   = Light::Framebuffer::Create("res/FramebuffersVS.shader", "res/InversePS.shader");
	m_Kernel    = Light::Framebuffer::Create("res/FramebuffersVS.shader", "res/KernelPS.shader");

	m_KernelData = Light::ConstantBuffer::Create(Light::ConstantBufferIndex_ClientSlot0, sizeof(float) * 16);


	glm::mat3 convolutionMatrix = glm::mat3(1.0, 1.0, 1.0,
	                                        1.0, -8.0, 1.0,
	                                        1.0, 1.0, 1.0);

	float* map = (float*)m_KernelData->Map();

	memcpy(map, glm::value_ptr(convolutionMatrix), sizeof(glm::mat3));
	*(map + 12) = 1.0f / Light::GraphicsContext::GetResolution().width;
	*(map + 13) = 1.0f / Light::GraphicsContext::GetResolution().height;

	m_KernelData->UnMap();
}

void QuadsLayer::OnDetatch()
{
	m_KernelData.reset();
	LT_TRACE("Detatched QuadLayer");
}

void QuadsLayer::OnUpdate(float DeltaTime)
{
	if (m_SelectedSprite)
		m_SelectedSprite->position = Light::Input::MousePosToCameraView(m_Camera) - m_SelectedSprite->size / 2.0f;

	if (Light::Input::GetKey(KEY_A))
		m_Camera->MoveX(-m_CameraSpeed * DeltaTime);
	if (Light::Input::GetKey(KEY_D))
		m_Camera->MoveX(m_CameraSpeed * DeltaTime);

	if (Light::Input::GetKey(KEY_W))
		m_Camera->MoveY(-m_CameraSpeed * DeltaTime);
	if (Light::Input::GetKey(KEY_S))
		m_Camera->MoveY(m_CameraSpeed * DeltaTime);
}

void QuadsLayer::OnRender()
{
	m_TextureArray->Bind();

	for (auto& sprite : m_Sprites)
		Light::Renderer::DrawQuad(sprite.position, sprite.size, sprite.coordinates, sprite.tint);
}

void QuadsLayer::ShowDebugWindow()
{
	if (ImGui::TreeNode("Camera"))
	{
		m_Camera->ShowDebugLayer();
		ImGui::BulletText("speed: %f", m_CameraSpeed);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sprites"))
	{
		ImGui::BulletText("you can drag sprites with mouse!");
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Post-Proccessing effects"))
	{
		if (ImGui::Button("grayscale"))
			Light::Renderer::AddFramebuffer(m_Grayscale);
		if (ImGui::Button("inverse"))
			Light::Renderer::AddFramebuffer(m_Inverse);
		if (ImGui::Button("edge detection"))
			Light::Renderer::AddFramebuffer(m_Kernel);

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
		m_Camera->Zoom(event.GetOffset() * 25);
	return true;
}

bool QuadsLayer::OnWindowResize(Light::WindowResizedEvent& event)
{
	m_Camera->SetProjection(Light::GraphicsContext::GetAspectRatio(), m_Camera->GetZoomLevel());

	m_Grayscale->Resize(event.GetWidth(), event.GetHeight());
	m_Inverse->Resize(event.GetWidth(), event.GetHeight());
	m_Kernel->Resize(event.GetWidth(), event.GetHeight());

	return true;
}