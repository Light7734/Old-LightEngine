#include "ltpch.h"
#include "CameraController.h"

namespace Light {

	CameraController::CameraController(const glm::vec2& position, float aspectRatio, float zoomLevel)
		: m_Position(position), m_AspectRatio(aspectRatio), m_ZoomLevel(zoomLevel), m_Bounds( {} )
	{
		UpdateCameraBounds();
	}

	void CameraController::UpdateCameraBounds()
	{
		m_Bounds.left = m_Position.x - (m_ZoomLevel * m_AspectRatio);
		m_Bounds.right = m_Position.x + (m_ZoomLevel * m_AspectRatio);
		m_Bounds.top = m_Position.y - m_ZoomLevel;
		m_Bounds.bottom = m_Position.y + m_ZoomLevel;
	}

}