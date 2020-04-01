#include "QuadsLayer.h"

QuadsLayer::QuadsLayer(std::shared_ptr<Light::Camera> camera)
	: m_Camera(camera), m_SelectedSprite(nullptr)
{
	m_LayeDebugrName = "QuadsLayer";

	// create texture array and load an atlas
	m_TextureArray = Light::TextureArray::Create(1u);

	m_TextureArray->CreateSliceWithAtlas("res/atlas.png", "QuadsLayer atlas", "res/atlas.txt");

	// create texture atlas to extract coordinates of texture array's slices
	std::shared_ptr<Light::TextureAtlas> textureAtlas;
	textureAtlas = m_TextureArray->GetAtlas("QuadsLayer atlas");
	Light::TextureCoordinates* boxCoords = textureAtlas->GetCoordinates("box");

	// create sprites
	for (int i = 0; i < 125; i++)
	{
		Sprite sprite;

		sprite.size = glm::vec2(100.0f, 100.0f);
		sprite.position.x = std::rand() % 1000;
		sprite.position.y = std::rand() % 1000;

		sprite.coordinates = boxCoords;

		m_Sprites.push_back(sprite);
	}
}

void QuadsLayer::OnAttach()
{
	LT_TRACE("Attached: {}", m_LayeDebugrName);
}

void QuadsLayer::OnDetatch()
{
	LT_TRACE("Detached: {}", m_LayeDebugrName);
}

void QuadsLayer::OnUpdate(float DeltaTime)
{
	if (m_SelectedSprite)
		m_SelectedSprite->position = Light::Input::MousePosToCameraView(m_Camera) - m_SelectedSprite->size / 2.0f;
}

void QuadsLayer::OnRender()
{
	m_TextureArray->Bind();

	for (const auto& sprite : m_Sprites)
		Light::Renderer::DrawQuad(sprite.position, sprite.size, sprite.coordinates, sprite.tint);
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

	for (auto& sprite : m_Sprites)
	{
		if (mouse.x > sprite.position.x&& mouse.x < sprite.position.x + sprite.size.x &&
			mouse.y > sprite.position.y&& mouse.y < sprite.position.y + sprite.size.y)
		{
			unsigned int distX = std::abs(mouse.x - (sprite.position.x + sprite.size.x / 2.0f));
			unsigned int distY = std::abs(mouse.y - (sprite.position.y + sprite.size.y / 2.0f));

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