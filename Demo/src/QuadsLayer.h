#pragma once

#include <LightEngine.h>

// struct to hold the information needed to render a quad.
struct Sprite
{
	glm::vec2 position;
	glm::vec2 size;
	glm::vec4 tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	Light::TextureCoordinates* uv;
};

class QuadsLayer : public Light::Layer
{
private:
	std::shared_ptr<Light::Camera> m_Camera;

	std::vector<Sprite> m_Sprites; // to render quads with
	Sprite* m_SelectedSprite; // to drag & drop with mouse

	// to rotate the sprites
	float m_Angle;
	bool b_BoundToTimer;
public:
	QuadsLayer(std::shared_ptr<Light::Camera> camera);
	~QuadsLayer();

	void OnAttach() override;
	void OnDetatch() override;
	
	void OnUpdate(float DeltaTime) override;
	void OnRender() override;

	void ShowDebugWindow() override;

	void OnEvent(Light::Event& event) override;
private:
	bool OnButtonPress  (Light::MouseButtonPressedEvent& event );
	bool OnButtonRelease(Light::MouseButtonReleasedEvent& event);
};