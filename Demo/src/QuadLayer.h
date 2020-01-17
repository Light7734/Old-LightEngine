#pragma once

#include <LightEngine.h>

class QuadLayer : public Light::Layer
{
private:
	glm::vec2 m_Position = glm::vec2(200.0f, 150.0f);
	glm::vec2 m_Size = glm::vec2(125.0f, 125.0f);
	glm::vec4 m_Color = glm::vec4(0.8f, 0.1f, 0.3f, 1.0f);

	float m_Speed = 250.0f;
public:
	void OnAttach() override
	{
		LT_TRACE("Attached QuadLayer");
	}

	void OnDetatch() override	
	{
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
		Light::Renderer::DrawQuad(m_Position, m_Size, m_Color);
	}

	inline const glm::vec2& GetPosition() { return m_Position; }
	inline const glm::vec2& GetSize    () { return m_Size;     }
};