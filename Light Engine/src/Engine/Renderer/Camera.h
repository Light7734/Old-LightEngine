#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

namespace Light {

	class Camera
	{
	private:
		const glm::vec3 m_Up;

		glm::vec2 m_Position;

		float m_AspectRatio;
		float m_ZoomLevel;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
	public:
		Camera(const glm::vec2& position, float aspectRatio, float zoomLevel)
			: m_Up(0.0f, 1.0f, 0.0f), m_Position(position), m_AspectRatio(aspectRatio), m_ZoomLevel(zoomLevel) {}

		void ShowDebugWindow();

		// getters
		const glm::mat4& GetView();
		const glm::mat4& GetProjection();

		inline const glm::vec2& GetPosition() const { return m_Position; }

		inline float GetZoomLevel  () const { return m_ZoomLevel;   }
		inline float GetAspectRatio() const { return m_AspectRatio; }

		// setters
		inline void Move (const glm::vec2& offset) { m_Position += offset;  }

		inline void MoveX(float offset) { m_Position.x += offset; }
		inline void MoveY(float offset) { m_Position.y += offset; }

		inline void Zoom(float offset) { m_ZoomLevel -= offset; }

		inline void SetPosition(const glm::vec2& position) { m_Position = position; }

		inline void SetZoomLevel(float zoom) { m_ZoomLevel = zoom; }

		inline void SetAspectRatio(float ratio) { m_AspectRatio = ratio; }

		inline void SetProjection(float aspectRatio, float zoomLevel)
			{ m_AspectRatio = aspectRatio; m_ZoomLevel = zoomLevel; }
	};

}