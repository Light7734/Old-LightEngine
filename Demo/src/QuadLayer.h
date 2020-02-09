#pragma once

#include <LightEngine.h>

struct Sprite
{
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 size;

	Light::TextureCoordinates* coordinates;
};

class QuadLayer : public Light::Layer
{
private:
	std::shared_ptr<Light::TextureAtlas> m_TextureAtlas;
	Sprite m_Sprites[128];
	float m_Speed = 250.0f;
public:
	void OnAttach() override
	{
		LT_TRACE("Attached QuadLayer");

		Light::TextureCoordinates *pepeCoords, *awesomeCoords;

		m_TextureAtlas = Light::TextureAtlas::Create("res/atlas.txt");

		pepeCoords = m_TextureAtlas->GetCoordinates("pepe");
		awesomeCoords = m_TextureAtlas->GetCoordinates("awesomeface");

		for (int i = 0; i < std::size(m_Sprites); i++)
		{
			Sprite& sprite = m_Sprites[i];

			if (i > std::size(m_Sprites) / 3)
				sprite.coordinates = awesomeCoords;
			else
				sprite.coordinates = pepeCoords;

			sprite.size = glm::vec2(rand() % ((125 - 25) + 1) + 25);
			sprite.position.x = std::rand() % Light::Window::GetWidth() - (sprite.size.x + 10.0f);
			sprite.position.y = std::rand() % Light::Window::GetHeight() - (sprite.size.y + 10.0f);

			sprite.velocity = glm::vec2(rand() % ((125 - 25) + 1) + 25);
		}
	}

	void OnDetatch() override	
	{
		LT_TRACE("Detatched QuadLayer");
	}
	
	void OnUpdate(float DeltaTime) override
	{
		for (int i = 0; i < std::size(m_Sprites); i++)
		{
			Sprite& sprite = m_Sprites[i];

			sprite.position += sprite.velocity * DeltaTime;

			if (sprite.position.x < 0.0f)
			{
				sprite.position.x = 0.0f;
				sprite.velocity.x *= -1;
			}
			if (sprite.position.x + sprite.size.x > Light::Window::GetWidth())
			{
				sprite.position.x = Light::Window::GetWidth() - sprite.size.x;
				sprite.velocity.x *= -1;
			}

			if (sprite.position.y < 0.0f)
			{
				sprite.position.y = 0.0f;
				sprite.velocity.y *= -1;
			}
			if (sprite.position.y + sprite.size.y > Light::Window::GetHeight())
			{
				sprite.position.y = Light::Window::GetHeight() - sprite.size.y;
				sprite.velocity.y *= -1;
			}
		}
	}

	void OnRender() override
	{
		static const glm::vec4 tint(1.0f);

		Light::Renderer::Start();
		for (int i = 0; i < std::size(m_Sprites); i++)
			Light::Renderer::DrawQuad(m_Sprites[i].position, m_Sprites[i].size, m_Sprites[i].coordinates, tint);
		Light::Renderer::End();
	} 

	inline const glm::vec2& GetPosition() { return m_Sprites[0].position; }
	inline const glm::vec2& GetSize    () { return m_Sprites[0].size;     }
};