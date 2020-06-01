#include "TextLayer.h"

TextLayer::TextLayer(std::shared_ptr<Light::Camera> camera)
	: m_Angle(-2.5f), m_Camera(camera), m_BlenderEnabled(true)
{
	LT_PROFILE_FUNC();
	LT_TRACE("TextLayer::TextLayer");
	m_LayeDebugrName = "TextLayer";

	// load Fonts
	Light::ResourceManager::LoadFont("arial", "res/arial.ttf", 24);
	Light::ResourceManager::LoadFont("comic", "res/comic.ttf", 24);
	Light::ResourceManager::LoadFont("impact", "res/impact.ttf", 24);

	// initialize Texts
	m_Arial.content = "The quick brown fox jumps over the lazy dog";
	m_Arial.font = Light::ResourceManager::GetFont("arial");
	m_Arial.position = glm::vec3(0.0f, -800.0f, m_DrawPriority);
	m_Arial.scale = 3.2f;

	m_Arial.tint = glm::vec4(255.0f / 255.0f, 57.0f / 255.0f , 57.0f / 255.0f, 1.0f);

	m_Comic.content = "The quick brown fox jumps over the lazy dog";
	m_Comic.font = Light::ResourceManager::GetFont("comic");
	m_Comic.scale = 3.2f;
	m_Comic.position = glm::vec3(0.0f, -700.0f, m_DrawPriority);

	m_Comic.tint = glm::vec4(14.0f / 255.0f, 163.0f / 255.0f, 255.0f / 255.0f, 1.0f);

	m_Impact.content = "The quick brown fox jumps over the lazy dog";
	m_Impact.font = Light::ResourceManager::GetFont("impact");
	m_Impact.position = glm::vec3(100.0f, -600.0f, m_DrawPriority);
	m_Impact.scale = 3.2f;

	m_Impact.tint = glm::vec4(0.0f / 255.0f, 220 / 255.0f, 146.0f / 255.0f, 1.0f);
}

TextLayer::~TextLayer()
{
	LT_PROFILE_FUNC();
	LT_TRACE("TextLayer::~TextLayer");

	// delete fonts
	Light::ResourceManager::DeleteFont("arial");
	Light::ResourceManager::DeleteFont("comic");
	Light::ResourceManager::DeleteFont("impact");
}

void TextLayer::OnAttach()
{
	LT_TRACE("TextLayer::OnAttach");
}

void TextLayer::OnDetatch()
{
	LT_TRACE("TextLayer::OnDetatch");
}

void TextLayer::OnRender()
{
	// all layers should specify whether their quads should be renderer with blending enabled or disabled because previous layers
	// can change the blending state.
	// note: you can't enable and disable this between DrawQuads, Renderer batches all the quads together to minimize render calls,
	// you can have only one blending state for each EndScene.
	if(m_BlenderEnabled)
		Light::Blender::Get()->Enable();
	else
		Light::Blender::Get()->Disable();

	Light::Renderer::BeginScene(m_Camera);
	Light::Renderer::DrawString("The quick brown fox jumps over the lazy dog", m_Arial.font,
	                            glm::vec3(m_Arial.position, m_DrawPriority), m_Arial.scale, m_Arial.tint);

	// again, do not call the function with angle parameter if it's always 0, calculating quad's vertices' rotated position is a bit costly.
	// this amplifies on rendering strings because each character counts as a separate quad.
	Light::Renderer::DrawString("The quick brown fox jumps over the lazy dog", m_Comic.font,
	                            glm::vec3(m_Comic.position, m_DrawPriority), glm::radians(m_Angle), m_Comic.scale, m_Comic.tint);

	Light::Renderer::DrawString("The quick brown fox jumps over the lazy dog", m_Impact.font,
	                            glm::vec3(m_Impact.position, m_DrawPriority), m_Impact.scale, m_Impact.tint);
	Light::Renderer::EndScene();
}

void TextLayer::ShowDebugWindow()
{
	ImGui::Checkbox("Blend", &m_BlenderEnabled);

	ImGui::BulletText("arial");
	ImGui::PushID("arial");
	m_Arial.ShowDebugWindow();
	ImGui::PopID();

	ImGui::BulletText("comic");
	ImGui::PushID("comic");
	ImGui::DragFloat("Angle", &m_Angle, 0.1f);
	m_Comic.ShowDebugWindow();
	ImGui::PopID();

	ImGui::BulletText("impact");
	ImGui::PushID("impact");
	m_Impact.ShowDebugWindow();
	ImGui::PopID();
}
