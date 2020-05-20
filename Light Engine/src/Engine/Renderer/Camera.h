#pragma once

#include "CameraController.h"

#include "Core/Core.h"

#include <glm/glm.hpp>

namespace Light {

	class Camera
	{
	private:
		const glm::vec3 m_Up;

		glm::mat4 m_View;
		glm::mat4 m_Projection;

		std::shared_ptr<CameraController> m_Controller;
	public:
		Camera(const glm::vec2& position, float aspectRatio, float zoomLevel);

		// getters
		inline std::shared_ptr<CameraController> GetController() { return m_Controller; }

		inline const CameraBounds& GetCameraBounds() { return m_Controller->GetCameraBounds(); }

		inline const glm::mat4& GetView() const { return m_View; }
		inline const glm::mat4& GetProjection() const { return m_Projection; }

		void CalculateViewProjection();
	};

}