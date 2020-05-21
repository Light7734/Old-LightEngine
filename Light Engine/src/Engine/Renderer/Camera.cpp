#include "ltpch.h"
#include "Camera.h"	

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

namespace Light {

	Camera::Camera(const glm::vec2& position, float aspectRatio, float zoomLevel)
		: m_Up(0.0f, 1.0f, 0.0f), m_View(1.0f), m_Projection(1.0f)
	{
		m_Controller = std::make_shared<CameraController>(position, aspectRatio, zoomLevel);
	}

	void Camera::CalculateViewProjection()
	{
		const glm::vec2& position = m_Controller->GetPosition();
		const float aspectRatio = m_Controller->GetAspectRatio();
		const float zoomLevel = m_Controller->GetZoomLevel();

		m_View = glm::lookAt(glm::vec3(position, 1.0f), glm::vec3(position, 0.0f), m_Up);

		m_Projection = glm::ortho(aspectRatio * -zoomLevel,
		                          aspectRatio *  zoomLevel,
		                                         zoomLevel,
		                                        -zoomLevel);
	}

	void Camera::ShowDebugWindow()
	{
		CameraBounds bounds = m_Controller->GetCameraBounds();


		ImGui::BulletText("position: %.2f, %.2f", m_Controller->GetPosition().x, m_Controller->GetPosition().y);
		ImGui::BulletText("ratio:    %.5f", m_Controller->GetAspectRatio());
		ImGui::BulletText("zoom:     %.1f", m_Controller->GetZoomLevel());
		ImGui::Text("bounds:");
		ImGui::BulletText("left, right: %.2f, %.2f", bounds.left, bounds.right);
		ImGui::BulletText("top, bottom: %.2f, %.2f", bounds.top, bounds.bottom);
		ImGui::BulletText("width - height: [%.2f x %.2f] ", bounds.GetWidth(), bounds.GetHeight());

	}

}