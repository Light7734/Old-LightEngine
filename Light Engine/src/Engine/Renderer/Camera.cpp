#include "ltpch.h"
#include "Camera.h"	

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Light {

	glm::mat4 Camera::s_Projection = glm::mat4(1.0f);
	glm::mat4 Camera::s_View = glm::mat4(1.0f);

	glm::vec3 Camera::s_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec2 Camera::s_Pos = glm::vec2(0.0f, 0.0f);

	void Camera::Move(const glm::vec2& offset)
	{
		s_Pos += offset;
		s_View = glm::lookAt(glm::vec3(s_Pos, 1.0f), glm::vec3(s_Pos, 0.0f), s_Up);
		ConstantBuffers::SetViewMatrix(&s_View);
	}

	void Camera::SetPosition(const glm::vec2& position)
	{
		s_Pos = position;
		s_View = glm::lookAt(glm::vec3(s_Pos, 1.0f), glm::vec3(s_Pos, 0.0f), s_Up);
		ConstantBuffers::SetViewMatrix(glm::value_ptr(s_View));
	}

	void Camera::SetProjection(float width, float height)
	{
		s_Projection = glm::ortho(0.0f, width, height, 0.0f, -10.0f, 10.0f);
		ConstantBuffers::SetProjMatrix(glm::value_ptr(s_Projection));
	}

	void Camera::UpdateConstants()
	{
		ConstantBuffers::SetViewMatrix(glm::value_ptr(s_View));
		ConstantBuffers::SetProjMatrix(glm::value_ptr(s_Projection));
	}

}