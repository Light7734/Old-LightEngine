#pragma once

#include <LightEngine.h>

struct Sprite
{
	glm::vec2 position;
	glm::vec2 size;
	glm::vec4 tint;

	Light::TextureCoordinates* coordinates;	
};

class QuadsLayer : public Light::Layer
{
private:
	Light::Camera m_Camera;
	float m_CameraSpeed;

	std::shared_ptr<Light::TextureAtlas> m_TextureAtlas;

	std::vector<Sprite> m_Sprites;
	Sprite* m_SelectedSprite;
public:
	QuadsLayer();

	void OnAttach() override;
	void OnDetatch() override;
	
	void OnUpdate(float DeltaTime) override;
	void OnRender() override;

	void ShowDebugWindow() override;

	void OnEvent(Light::Event& event) override;

private:
	bool OnButtonPress  (Light::MouseButtonPressedEvent& event );
	bool OnButtonRelease(Light::MouseButtonReleasedEvent& event);
	bool OnMouseScroll  (Light::MouseScrolledEvent& event      );
	bool OnWindowResize (Light::WindowResizedEvent& event      );

	void DoCollision(Sprite& sprite, Sprite& target);
};