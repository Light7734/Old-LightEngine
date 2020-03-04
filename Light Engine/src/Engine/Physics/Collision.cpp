#include "ltpch.h"
#include "Collision.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ImGui.h>

namespace Light {


	bool CheckCollision(const glm::vec2& rectOnePos, const glm::vec2& rectOneSize,
	                    const glm::vec2& rectTwoPos, const glm::vec2& rectTwoSize,
	                    glm::vec2* outPenetration)
	{
		bool collisionX = rectOnePos.x + rectOneSize.x >= rectTwoPos.x &&
		                  rectTwoPos.x + rectTwoSize.x >= rectOnePos.x;
		if (!collisionX)
			return false;

		bool collisionY = rectOnePos.y + rectOneSize.y >= rectTwoPos.y &&
		                  rectTwoPos.y + rectTwoSize.y >= rectOnePos.y;
		if (!collisionY)
			return false;

		glm::vec2 rectOneCenter = rectOnePos + rectOneSize / 2.0f;
		glm::vec2 rectTwoCenter = rectTwoPos + rectTwoSize / 2.0f;

		glm::vec2 lineBetweenCenteres = rectOneCenter - rectTwoCenter;
		glm::vec2 direction = glm::normalize(lineBetweenCenteres);
		glm::vec2 absoluteDirection = glm::abs(direction);


		// penetrating from
		if (absoluteDirection.x > absoluteDirection.y) // horizontally
		{
			float normalDist = (rectOneSize.x + rectTwoSize.x) / 2.0f;
			float peneteration = normalDist - std::abs(lineBetweenCenteres.x);

			if (direction.x > 0) // right
				*outPenetration = glm::vec2(-peneteration, 0.0f);
			else // left
				*outPenetration = glm::vec2(+peneteration, 0.0f);
		}
		else // vertically
		{
			float normalDist = (rectOneSize.y + rectTwoSize.y) / 2.0f;
			float peneteration = normalDist - std::abs(lineBetweenCenteres.y);

			if (direction.y > 0) // down
				*outPenetration = glm::vec2(0.0f, -peneteration);
			else // up
				*outPenetration = glm::vec2(0.0f, +peneteration);
		}

		return true;
	}

	bool CheckCollision(const glm::vec2& rectPos, const glm::vec2& rectSize,
	                    const glm::vec2& circlePos, float circleRadi,
	                    glm::vec2* outPenetration)
	{
		const glm::vec2 circleCenter = circlePos + circleRadi;
		const glm::vec2 rectHalfExtends = rectSize / 2.0f;
		const glm::vec2 rectCenter = rectPos + rectHalfExtends;

		const glm::vec2 lineBetweenCenters = circleCenter - rectCenter;
		const glm::vec2 closestPoint = rectCenter + glm::clamp(lineBetweenCenters, -rectHalfExtends, +rectHalfExtends);

		const float distanceToPoint = glm::length(closestPoint - circleCenter);

		if (distanceToPoint > circleRadi)
  			return false;

		*outPenetration = -((distanceToPoint - circleRadi) * glm::normalize(lineBetweenCenters));
		return true;
	}

	bool CheckCollision(const glm::vec2& circlePos, float circleRadi,
	                    const glm::vec2& rectPos, const glm::vec2& rectSize,
	                    glm::vec2* outPenetration)
	{
		const glm::vec2 circleCenter = circlePos + circleRadi;
		const glm::vec2 rectHalfExtends = rectSize / 2.0f;
		const glm::vec2 rectCenter = rectPos + rectHalfExtends;

		const glm::vec2 lineBetweenCenters = circleCenter - rectCenter;
		const glm::vec2 closestPoint = rectCenter + glm::clamp(lineBetweenCenters, -rectHalfExtends, +rectHalfExtends);

		const float distanceToPoint = glm::length(closestPoint - circleCenter);

		if (distanceToPoint > circleRadi)
  			return false;

		*outPenetration = (distanceToPoint - circleRadi) * glm::normalize(lineBetweenCenters);
		return true;
	}

	bool CheckCollision(const glm::vec2& circleOnePos, float circleOneRadi,
	                    const glm::vec2& circleTwoPos, float circleTwoRadi,
	                    glm::vec2* outPenetration)
	{
		const glm::vec2 circleOneCenter  = circleOnePos + circleOneRadi;
		const glm::vec2 circleTworCenter = circleTwoPos + circleTwoRadi;

		const glm::vec2 lineBetweenCenters = circleOneCenter - circleTworCenter;

		const float penetration = glm::length(lineBetweenCenters) - circleOneRadi - circleTwoRadi;

		if (penetration > 0.0f)
			return false;

		*outPenetration = penetration * glm::normalize(lineBetweenCenters);
		return true;
	}

}