#include "ltpch.h"
#include "CameraController.h"

namespace Light {

	CameraController::CameraController(const glm::vec2& position, float aspectRatio, float zoomLevel)
		: m_Position(position), m_AspectRatio(aspectRatio), m_ZoomLevel(zoomLevel)
	{
	}

}