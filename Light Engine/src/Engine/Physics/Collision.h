#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

namespace Light {

	// AABB - AABB
	bool CheckCollision(const glm::vec2& rectOnePos, const glm::vec2& rectOneSize,
	                    const glm::vec2& rectTwoPos, const glm::vec2& rectTwoSize,
	                    glm::vec2* outPenetration);

	// AABB - Circle
	bool CheckCollision(const glm::vec2& rectPos, const glm::vec2& rectSize,
	                    const glm::vec2& circlePos, float circleRadi,
                        glm::vec2* outPenetration);
		
	// Circle - AABB
	bool CheckCollision(const glm::vec2& circlePos, float circleRadi,
	                    const glm::vec2& rectPos, const glm::vec2& rectSize,
	                    glm::vec2* outPenetration);

	// Circle - Circle
	bool CheckCollision(const glm::vec2& circleOnePos, float circleOneRadi,
	                    const glm::vec2& circleTwoPos, float circleTwoRadi,
	                    glm::vec2* outPenetration);
}