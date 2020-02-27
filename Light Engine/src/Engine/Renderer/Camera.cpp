#include "ltpch.h"
#include "Camera.h"	

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

namespace Light {

	const glm::vec3 Camera::s_Up = glm::vec3(0.0f, 1.0f, 0.0f);

	const glm::f32* Camera::GetView()
	{
		m_View = glm::lookAt(glm::vec3(m_Position, 1.0f), glm::vec3(m_Position, 0.0f), s_Up);
		return glm::value_ptr(m_View);
	}

	const glm::f32* Camera::GetProjection()
	{
		m_Projection = glm::ortho(-m_AspectRatio * m_ZoomLevel,
		                           m_AspectRatio * m_ZoomLevel,
		                                           m_ZoomLevel,
		                                          -m_ZoomLevel);
		return glm::value_ptr(m_Projection);
	}

	void Camera::ShowDebugLayer()
	{
		ImGui::Begin("Light::Camera");

		ImGui::Text("Position: [%f x %f]", m_Position.x, m_Position.y);
		ImGui::Text("Zoom level: %f", m_ZoomLevel);
		ImGui::Text("Aspect ratio: %f", m_AspectRatio);

		ImGui::End();
	}
}