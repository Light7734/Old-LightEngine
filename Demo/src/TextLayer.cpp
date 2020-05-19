#include "TextLayer.h"

TextLayer::TextLayer(std::shared_ptr<Light::Camera> camera)
	: m_Angle(-2.5f), m_Camera(camera)
{
	LT_PROFILE_FUNC();

	m_LayeDebugrName = "TextLayer";

	Light::FontManager::LoadFont("arial", "res/arial.ttf", 24);
	Light::FontManager::LoadFont("comic", "res/comic.ttf", 24);
	Light::FontManager::LoadFont("impact", "res/impact.ttf", 24);

	m_Arial.font = Light::FontManager::GetFont("arial");
	m_Arial.position = glm::vec2(100.0f, -100.0f);
	m_Arial.scale = 3.2f;
	// 255, 57, 57
	m_Arial.tint = glm::vec4(255.0f / 255.0f, 57.0f / 255.0f , 57.0f / 255.0f, 1.0f);

	m_Comic.font = Light::FontManager::GetFont("comic");
	m_Comic.scale = 3.2f;
	m_Comic.position = glm::vec2(100.0f, 0.0f);
	// 14, 163, 255
	m_Comic.tint = glm::vec4(14.0f / 255.0f, 163.0f / 255.0f, 255.0f / 255.0f, 1.0f);

	m_Impact.font = Light::FontManager::GetFont("impact");
	m_Impact.position = glm::vec2(100.0f, 100.0f);
	m_Impact.scale = 3.2f;
	// 88, 220, 146
	m_Impact.tint = glm::vec4(88.0f / 255.0f, 220 / 255.0f, 146.0f / 255.0f, 1.0f);
}

void TextLayer::OnAttach()
{
	LT_TRACE("Attached: {}", m_LayeDebugrName);
}

void TextLayer::OnDetatch()
{
	LT_TRACE("Detached: {}", m_LayeDebugrName);
}

void TextLayer::OnRender()
{
	if(m_BlenderEnabled)
		Light::Blender::Get()->Enable();
	else
		Light::Blender::Get()->Disable();

	Light::Renderer::BeginScene(m_Camera);
	Light::Renderer::DrawString("The quick brown fox jumps over the lazy dog", m_Arial.font, m_Arial.position, m_Arial.scale, m_Arial.tint);
	Light::Renderer::DrawString("The quick brown fox jumps over the lazy dog", m_Comic.font, m_Comic.position, glm::radians(m_Angle), m_Comic.scale, m_Comic.tint);
	Light::Renderer::DrawString("The quick brown fox jumps over the lazy dog", m_Impact.font, m_Impact.position, m_Impact.scale, m_Impact.tint);
	Light::Renderer::EndScene();
}

void TextLayer::ShowDebugWindow()
{
	ImGui::DragFloat("Angle", &m_Angle, 0.1f);
	ImGui::Checkbox("Blend", &m_BlenderEnabled);

	ImGui::BulletText("arial");
	ImGui::PushID("arial");
	m_Arial.ShowDebugWindow();
	ImGui::PopID();

	ImGui::BulletText("comic");
	ImGui::PushID("comic");
	m_Comic.ShowDebugWindow();
	ImGui::PopID();

	ImGui::BulletText("impact");
	ImGui::PushID("impact");
	m_Impact.ShowDebugWindow();
	ImGui::PopID();
}
