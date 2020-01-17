#pragma once

#include "Buffers.h"

#include <glm/glm.hpp>

namespace Light {

	class Camera
	{
	private:
		static glm::mat4 s_Projection;
		static glm::mat4 s_View;

		static glm::vec3 s_Up;
		static glm::vec2 s_Pos;
	public:
		Camera() = delete;

		static void Move(const glm::vec2& offset);

		static void SetPosition(const glm::vec2& position);
		static void SetProjection(float width, float height);
		
		static void UpdateConstants();
	};

}