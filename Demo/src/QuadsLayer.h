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
	std::shared_ptr<Light::Camera> m_Camera;
	float m_CameraSpeed;

	std::shared_ptr<Light::TextureArray> m_TextureArray;
		
	std::vector<Sprite> m_Sprites;
	Sprite* m_SelectedSprite;

	std::shared_ptr<Light::Framebuffer> m_Grayscale;
	std::shared_ptr<Light::Framebuffer> m_Inverse;
	std::shared_ptr<Light::Framebuffer> m_Kernel;

	std::shared_ptr<Light::ConstantBuffer> m_KernelData;
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
};