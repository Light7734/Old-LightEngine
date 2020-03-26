#pragma once

#include <LightEngine.h>

class PostProcessLayer : public Light::Layer
{
private:
	bool m_GrayscaleAttached, m_InverseAttached, m_KernelAttached;

	std::shared_ptr<Light::Framebuffer> m_Grayscale, m_Inverse, m_Kernel;

	std::shared_ptr<Light::ConstantBuffer> m_KernelData;
	glm::mat3 m_ConvolutionMatrix;
public:
	PostProcessLayer();

	void OnAttach() override;
	void OnDetatch() override;

	void ShowDebugWindow() override;

	void OnEvent(Light::Event& event) override;
private:
	bool OnWindowResize(Light::WindowResizedEvent& event);
};