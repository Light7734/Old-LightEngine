#include "TextLayer.h"

TextLayer::TextLayer()
{
	LT_PROFILE_FUNC();
	m_LayeDebugrName = "TextLayer";

	Light::FontManager::LoadFont("arial", "res/arial.ttf", 24);
	Light::FontManager::LoadFont("comic", "res/comic.ttf", 24);
	Light::FontManager::LoadFont("impact", "res/impact.ttf", 24);

	m_Arial.font = Light::FontManager::GetFont("arial");
	m_Arial.position = glm::vec2(0.0f, -100.0f);
	m_Arial.scale = 3.2f;
	m_Arial.tint = glm::vec4(1.0f);

	m_Comic.font = Light::FontManager::GetFont("comic");
	m_Comic.position = glm::vec2(0.0f, 0.0f);
	m_Comic.scale = 3.2f;
	m_Comic.tint = glm::vec4(1.0f);

	m_Impact.font = Light::FontManager::GetFont("impact");
	m_Impact.position = glm::vec2(0.0f, 100.0f);
	m_Impact.scale = 3.2f;
	m_Impact.tint = glm::vec4(1.0f);
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
	Light::Renderer::DrawString("The quick brown fox jumps over the lazy dog [ arial ]", m_Arial.font, m_Arial.position, m_Arial.scale, m_Arial.tint);
	Light::Renderer::DrawString("The quick brown fox jumps over the lazy dog [ comics ]", m_Comic.font, m_Comic.position, m_Comic.scale, m_Comic.tint);
	Light::Renderer::DrawString("The quick brown fox jumps over the lazy dog [ impact ]", m_Impact.font, m_Impact.position, m_Impact.scale, m_Impact.tint);
}

void TextLayer::ShowDebugWindow()
{
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
