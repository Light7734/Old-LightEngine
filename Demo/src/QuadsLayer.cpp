#include "QuadsLayer.h"

QuadsLayer::QuadsLayer(std::shared_ptr<Light::Camera> camera)
	: m_Camera(camera), m_SelectedSprite(nullptr), b_BoundToTimer(true)
{
	LT_PROFILE_FUNC();
	LT_TRACE("QuadsLayer::QuadsLayer");
	m_LayeDebugrName = "QuadsLayer";

	// load texture atlas
	Light::ResourceManager::LoadTextureAtlas("QuadsLayerAtlas", "res/atlas.png", "res/atlas.txt");
	// call ResolceTextures after you've loaded all textures / texture atlases.
	Light::ResourceManager::ResolveTextures(); 

	// get texture atlas (it doesn't matter if it has an atlas or not, all textures can be retrived with ResourceManager::GetTexture).
	std::shared_ptr<Light::Texture> atlas = Light::ResourceManager::GetTexture("QuadsLayerAtlas");
	// get atlas's SubTexture's texture coordinates
	Light::TextureCoordinates* awesomefaceUV = atlas->GetSubTextureUV("awesomeface");

	// create sprites
	for (int i = 0; i < 125; i++)
	{
		Sprite sprite;
		
		sprite.size = glm::vec2(100.0f, 100.0f);
		sprite.position.x = 500.0f - std::rand() % 1000;
		sprite.position.y = 500.0f - std::rand() % 1000;
		
		sprite.uv = awesomefaceUV;
		
		m_Sprites.push_back(sprite);
	}
}

QuadsLayer::~QuadsLayer()
{
	LT_PROFILE_FUNC();
	LT_TRACE("QuadsLayer::~QuadsLayer");

	// delete texture (doesn't matter if it's texture atlas or a simple texture, all of them are deleted by ResourceManager::DeleteTexture).
	Light::ResourceManager::DeleteTexture("QuadsLayerAtlas");
}

void QuadsLayer::OnAttach()
{
	LT_TRACE("QuadsLayer::OnAttach");
}

void QuadsLayer::OnDetatch()
{
	LT_TRACE("QuadsLayer::OnDetatch");
}

void QuadsLayer::OnUpdate(float DeltaTime)
{
	static Light::Timer timer;

	if(b_BoundToTimer)
		m_Angle = timer.ElapsedTime() * 25.0f;

	if (m_SelectedSprite)
		m_SelectedSprite->position = glm::vec3(Light::Input::MousePosToCameraView(m_Camera), m_DrawPriority); // converts mouse pos to world pos
}

void QuadsLayer::OnRender()
{
	// ** all Renderer functions should be called in OnRender() function because it is wrapped with Renderer::Begin/EndFrame.

	// all layers should specify whether their quads should be renderer with blending enabled or disabled because previous layers
	// can change the blending state.
	// note: you can't enable and disable this between DrawQuads, Renderer batches all the quads together to minimize render calls,
	// you can have only one blending state for each EndScene.
	Light::Blender::Get()->Enable();

	// we have to call BeginScene before any drawing session
	Light::Renderer::BeginScene(m_Camera);

	// m_DrawPriority 
	// note: do not use DrawQuad with angle parameter if the angle is always 0,
	// calculating quad's vertices' rotated position is a bit costly.
	for (const auto& sprite : m_Sprites)
		Light::Renderer::DrawQuad(glm::vec3(sprite.position, m_DrawPriority), sprite.size, glm::radians(m_Angle), sprite.uv, sprite.tint); 

	// we have to call EndScene before another BeginScene, otherwise it results in mapping Vertexbuffer twice without
	// unmapping it, which results in a runtime error.
	Light::Renderer::EndScene();
}

void QuadsLayer::ShowDebugWindow()
{
	ImGui::Checkbox("Bound to timer", &b_BoundToTimer);
	ImGui::DragFloat("Angle", &m_Angle, 1.0f);
}

void QuadsLayer::OnEvent(Light::Event& event)
{
	Light::Dispatcher dispatcher(event);

	dispatcher.Dispatch<Light::MouseButtonPressedEvent>(LT_EVENT_FN(QuadsLayer::OnButtonPress));
	dispatcher.Dispatch<Light::MouseButtonReleasedEvent>(LT_EVENT_FN(QuadsLayer::OnButtonRelease));
}

bool QuadsLayer::OnButtonPress(Light::MouseButtonPressedEvent& event)
{
	glm::vec2 mouse = Light::Input::MousePosToCameraView(m_Camera);

	Sprite* bestMatch = nullptr;
	unsigned int closestDist = -1.0f;

	// find sprite closest to mouse
	for (auto& sprite : m_Sprites)
	{
		if (mouse.x > sprite.position.x  - sprite.size.x / 2.0f && mouse.x < sprite.position.x + sprite.size.x / 2 &&
			mouse.y > sprite.position.y - sprite.size.y / 2.0f && mouse.y < sprite.position.y + sprite.size.y / 2)
		{
			unsigned int distX = std::abs(mouse.x - sprite.position.x);
			unsigned int distY = std::abs(mouse.y - sprite.position.y);

			if (distX + distY < closestDist)
			{
				closestDist = distX + distY;
				bestMatch = &sprite;
			}
		}
	}

	m_SelectedSprite = bestMatch;
	return false;
}

bool QuadsLayer::OnButtonRelease(Light::MouseButtonReleasedEvent& event)
{
	m_SelectedSprite = nullptr;
	return false;
}