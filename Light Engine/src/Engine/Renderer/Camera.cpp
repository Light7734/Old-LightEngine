#include "ltpch.h"
#include "Camera.h"	

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

namespace Light {

	void Camera::ShowDebugLayer()
	{
		ImGui::BulletText("position: [%f x %f]", m_Position.x, m_Position.y);
		ImGui::BulletText("zoom level: %f", m_ZoomLevel);
		ImGui::BulletText("aspect ratio: %f", m_AspectRatio);
	}

	const glm::mat4& Camera::GetView()
	{
		m_View = glm::lookAt(glm::vec3(m_Position, 1.0f), glm::vec3(m_Position, 0.0f), m_Up);
		return m_View;
	}

	const glm::mat4& Camera::GetProjection()
	{
		m_Projection = glm::ortho(-m_AspectRatio * m_ZoomLevel,
		                           m_AspectRatio * m_ZoomLevel,
		                                           m_ZoomLevel,
		                                          -m_ZoomLevel);
		return m_Projection;
	}

}