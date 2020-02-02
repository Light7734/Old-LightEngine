#pragma once

#include <LightEngine.h>

class QuadLayer : public Light::Layer
{
private:
	std::shared_ptr<Light::Texture> m_Texture;

	glm::vec2 m_Position = glm::vec2(50.0f, 100.0f);
	const glm::vec2 m_RedSize = glm::vec2(25.0f, 25.0f);
	glm::vec2 m_Size = glm::vec2(100.0f, 100.0f);
	glm::vec4 m_Color = glm::vec4(0.7f, 0.4f, 0.4f, 1.0f);
	
	float m_Speed = 250.0f;
public:
	void OnAttach() override
	{
		LT_TRACE("Attached QuadLayer");

		m_Texture = Light::Texture::Create("res/awesomeface.png");
		Light::Renderer::AttachTexture(m_Texture);
	}

	void OnDetatch() override	
	{
		Light::Renderer::DetatchTexture(m_Texture);

		LT_TRACE("Detatched  QuadLayer");
	}
	
	void OnUpdate(float DeltaTime) override
	{
		if (Light::Input::GetKey(KEY_D))
			m_Position.x += m_Speed * DeltaTime;
		if (Light::Input::GetKey(KEY_A))
			m_Position.x -= m_Speed * DeltaTime;
		if (Light::Input::GetKey(KEY_S))
			m_Position.y += m_Speed * DeltaTime;
		if (Light::Input::GetKey(KEY_W))
			m_Position.y -= m_Speed * DeltaTime;

	}

	void OnRender() override
	{
		Light::Renderer::DrawQuad(glm::vec2(50.0f , 50.0f), m_RedSize, { 0.1f, 0.0f, 0.0f, 1.0f });
		Light::Renderer::DrawQuad(glm::vec2(100.0f, 50.0f), m_RedSize, { 0.2f, 0.0f, 0.0f, 1.0f });
		Light::Renderer::DrawQuad(glm::vec2(150.0f, 50.0f), m_RedSize, { 0.3f, 0.0f, 0.0f, 1.0f });
		Light::Renderer::DrawQuad(glm::vec2(200.0f, 50.0f), m_RedSize, { 0.4f, 0.0f, 0.0f, 1.0f });
		Light::Renderer::DrawQuad(glm::vec2(250.0f, 50.0f), m_RedSize, { 0.5f, 0.0f, 0.0f, 1.0f });
		Light::Renderer::DrawQuad(glm::vec2(300.0f, 50.0f), m_RedSize, { 0.6f, 0.0f, 0.0f, 1.0f });
		Light::Renderer::DrawQuad(glm::vec2(350.0f, 50.0f), m_RedSize, { 0.7f, 0.0f, 0.0f, 1.0f });
		Light::Renderer::DrawQuad(glm::vec2(400.0f, 50.0f), m_RedSize, { 0.8f, 0.0f, 0.0f, 1.0f });
		Light::Renderer::DrawQuad(glm::vec2(450.0f, 50.0f), m_RedSize, { 0.9f, 0.0f, 0.0f, 1.0f });
		Light::Renderer::DrawQuad(glm::vec2(500.0f, 50.0f), m_RedSize, { 1.0f, 0.0f, 0.0f, 1.0f });

		Light::Renderer::DrawQuad(m_Position, m_Size, m_Texture, {0.0f, 0.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f });
	} 

	void OnEvent(Light::Event& event)
	{
		Light::Dispatcher dispatcher(event);
		dispatcher.Dispatch<Light::MouseScrolledEvent>(LT_EVENT_FN(QuadLayer::OnMouseScroll));
	}

	bool OnMouseScroll(Light::MouseScrolledEvent& event)
	{
		m_Size += event.GetOffset() * 6.25f;
		return true;
	}

	inline const glm::vec2& GetPosition() { return m_Position; }
	inline const glm::vec2& GetSize    () { return m_Size;     }
};