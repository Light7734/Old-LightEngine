#pragma once

#include "Buffers.h"

#include <glm/glm.hpp>

namespace Light {

	class Camera
	{
	private:
		static const glm::vec3 s_Up;
	private:
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::vec2 m_Position;

		float m_ZoomLevel;
		float m_AspectRatio;
	public:
		Camera(const glm::vec2& position, float aspectRatio, float zoomLevel)
			: m_Position(position), m_AspectRatio(aspectRatio), m_ZoomLevel(zoomLevel) {}

		// Getters
		const glm::f32* GetView();
		const glm::f32* GetProjection();

		inline const glm::vec2& GetPosition() const { return m_Position; }

		inline float GetZoomLevel  () const { return m_ZoomLevel;   }
		inline float GetAspectRatio() const { return m_AspectRatio; }

		// Setters
		inline void Move (const glm::vec2& offset) { m_Position += offset;  }
		inline void MoveX(float            offset) { m_Position.x += offset; }
		inline void MoveY(float            offset) { m_Position.y += offset; }

		inline void Zoom(float offset) { m_ZoomLevel -= offset; }


		inline void SetPosition(const glm::vec2& position) { m_Position = position; }

		inline void SetProjection(float aspectRatio, float zoomLevel)
			{ m_AspectRatio = aspectRatio; m_ZoomLevel = zoomLevel; }

		inline void SetZoomLevel(float zoom) { m_ZoomLevel = zoom; }

		inline void SetAspectRatio(float ratio) { m_AspectRatio = ratio; }
	};

}