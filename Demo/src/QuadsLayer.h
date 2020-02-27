#pragma once

#include <LightEngine.h>

struct Sprite
{
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 size;

	Light::TextureCoordinates* coordinates;	
};

class QuadsLayer : public Light::Layer
{
private:
	Light::Camera m_Camera;
	float m_CameraSpeed = 250.0f;

	std::shared_ptr<Light::TextureAtlas> m_TextureAtlas;
	Sprite m_Sprites[128];
	Sprite m_Border;
public:
	QuadsLayer()
		: m_Camera(glm::vec2(500.0f, 500.0f), Light::GraphicsContext::GetAspectRatio(), 1000.0f)
	{
		m_LayerName = "QuadsLayer";

		m_Border.position = glm::vec2(-28, -62);
		m_Border.size = glm::vec2(1000.0f + 56, 1000.0f + 93);

		srand(time(NULL));

		for (int i = 0; i < std::size(m_Sprites); i++)
		{
			m_Sprites[i].size = glm::vec2(rand() % ((125 - 25) + 1) + 25);
			m_Sprites[i].position.x = std::rand() % 1000 - (m_Sprites[i].size.x + 10.0f);
			m_Sprites[i].position.y = std::rand() % 1000 - (m_Sprites[i].size.y + 10.0f);

			m_Sprites[i].velocity = glm::vec2(rand() % ((125 - 25) + 1) + 25);
		}
	}

	void OnAttach() override
	{
		LT_TRACE("Attached QuadLayer");

		m_Camera.SetProjection(Light::GraphicsContext::GetAspectRatio(), m_Camera.GetZoomLevel());

		Light::TextureCoordinates *pepeCoords, *awesomeCoords;
		m_TextureAtlas = Light::TextureAtlas::Create("res/atlas.txt");

		pepeCoords = m_TextureAtlas->GetCoordinates("pepe");
		awesomeCoords = m_TextureAtlas->GetCoordinates("awesomeface");
		m_Border.coordinates = m_TextureAtlas->GetCoordinates("border");

		for (int i = 0; i < std::size(m_Sprites); i++)
		{
			if (i > std::size(m_Sprites) / 3)
				m_Sprites[i].coordinates = awesomeCoords;
			else
				m_Sprites[i].coordinates = pepeCoords;
		}
	}

	void OnDetatch() override	
	{
		LT_TRACE("Detatched QuadLayer");
	}
	
	void OnUpdate(float DeltaTime) override
	{
		m_Sprites[0].position = Light::Input::MousePosToCameraView(m_Camera) - m_Sprites[0].size / 2.0f;

		if (Light::Input::GetKey(KEY_A))
			m_Camera.MoveX(-m_CameraSpeed * DeltaTime);
		if (Light::Input::GetKey(KEY_D))
			m_Camera.MoveX(m_CameraSpeed * DeltaTime);

		if (Light::Input::GetKey(KEY_W))
			m_Camera.MoveY(-m_CameraSpeed * DeltaTime);
		if (Light::Input::GetKey(KEY_S))
			m_Camera.MoveY(m_CameraSpeed * DeltaTime);


		for (int i = 1; i < std::size(m_Sprites); i++)
		{
			Sprite& sprite = m_Sprites[i];

			sprite.position += sprite.velocity * DeltaTime;

			if (sprite.position.x < 0.0f)
			{
				sprite.position.x = 0.0f;
				sprite.velocity.x *= -1;
			}
			else if (sprite.position.x + sprite.size.x > 1000.0f)
			{
				sprite.position.x = 1000.0f - sprite.size.x;
				sprite.velocity.x *= -1;
			}

			if (sprite.position.y < 0.0f)
			{
				sprite.position.y = 0.0f;
				sprite.velocity.y *= -1;
			}
			else if (sprite.position.y + sprite.size.y > 1000.0f)
			{
				sprite.position.y = 1000.0f - sprite.size.y;
				sprite.velocity.y *= -1;
			}
		}
	}

	void OnRender() override
	{
		Light::Renderer::Start(m_Camera); // Start


		for (int i = 0; i < std::size(m_Sprites); i++) // Draw ...
			Light::Renderer::DrawQuad(m_Sprites[i].position, m_Sprites[i].size, m_Sprites[i].coordinates, glm::vec4(1.0f));

		Light::Renderer::DrawQuad(m_Border.position, m_Border.size, m_Border.coordinates, glm::vec4(1.0f)); // ... Draw


		Light::Renderer::End(); // End
	} 

	void OnUserInterfaceUpdate()
	{
		m_Camera.ShowDebugLayer();
	}

	void OnEvent(Light::Event& event)
	{
		Light::Dispatcher dispatcher(event);

		dispatcher.Dispatch<Light::MouseScrolledEvent>(LT_EVENT_FN(QuadsLayer::OnMouseScroll));
		dispatcher.Dispatch<Light::WindowResizedEvent>(LT_EVENT_FN(QuadsLayer::OnWindowResize));
	}

	bool OnMouseScroll(Light::MouseScrolledEvent& event)
	{
		m_Camera.Zoom(event.GetOffset() * 25);
		return true;
	}

	bool OnWindowResize(Light::WindowResizedEvent& event)
	{
		m_Camera.SetProjection(Light::GraphicsContext::GetAspectRatio(), m_Camera.GetZoomLevel());
		return true;
	}

	inline const glm::vec2& GetPosition() { return m_Sprites[std::size(m_Sprites) - 1].position; }
	inline const glm::vec2& GetSize    () { return m_Sprites[std::size(m_Sprites) - 1].size;     }
};