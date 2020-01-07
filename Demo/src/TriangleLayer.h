#pragma once

#include <LightEngine.h>

class TriangleLayer : public Light::Layer
{
private:
	std::shared_ptr<Light::BufferLayout> m_Layout;
	std::shared_ptr<Light::VertexBuffer> m_Buffer;
	std::shared_ptr<Light::Shader> m_Shader;

public:
	TriangleLayer() = default;

	void OnAttach() override
	{
		float vertices[] =
		{
			 0.0f,  0.5f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<Light::VertexLayout> layout = Light::VertexLayout::Create({ {"Position", Light::VertexType::Float2},
																					{"Color"   , Light::VertexType::Float3} });

		m_Shader = Light::Shader::Create("res/vertex.shader", "res/fragment.shader");
		m_Buffer = Light::VertexBuffer::Create(vertices, sizeof(vertices), sizeof(float) * 5);
		m_Layout = Light::BufferLayout::Create(m_Shader, m_Buffer, layout);
	}

	void OnRender() override
	{
		m_Shader->Bind();
		m_Layout->Bind();
		m_Buffer->Bind();

		Light::RenderCommand::Draw(3);
	}

};