#pragma once

#include <LightEngine.h>

class TextLayer : public Light::Layer
{
private:
	// struct to hold the information needed to draw a string (usually you wouldn't want to do it this way).
	struct Text
	{
		std::shared_ptr<Light::Font> font;

		std::string content;
		glm::vec2 position;
		float scale;
		glm::vec4 tint;

		void ShowDebugWindow()
		{
			ImGui::DragFloat2("position", &position[0], 0.5f, NULL, NULL, "%.0f");
			ImGui::DragFloat("scale", &scale, 0.05f, NULL, NULL, "%.2f");
			ImGui::ColorEdit4("tint", &tint[0]);
		}
	};

	Text m_Arial;
	Text m_Comic;
	Text m_Impact;

	float m_Angle;
	bool m_BlenderEnabled;

	std::shared_ptr<Light::Camera> m_Camera;
public:
	TextLayer(std::shared_ptr<Light::Camera> camera);
	~TextLayer();

	void OnAttach() override;
	void OnDetatch() override;

	void OnRender() override;

	void ShowDebugWindow() override;
};