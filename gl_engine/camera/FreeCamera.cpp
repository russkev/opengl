#include "FreeCamera.h"

#include "../utils/VectorUtils.h"

#include <glm/gtc/matrix_transform.hpp>

namespace gl_engine
{
	const float FreeCamera::MOVE_SPEED = 0.5f;

	FreeCamera::FreeCamera()
	{}

	// // ----- CAMERA MOVEMENT ----- // //
	void FreeCamera::update()
	{
		auto const keyboardState = SDL_GetKeyboardState(nullptr);

		if (keyboardState[SDL_SCANCODE_A])
		{
			Camera::add_to_position(glm::vec3{ +MOVE_SPEED, 0, 0 });
		}
		if (keyboardState[SDL_SCANCODE_D])
		{
			Camera::add_to_position(glm::vec3{ -MOVE_SPEED, 0, 0 });
		}
		if (keyboardState[SDL_SCANCODE_W])
		{
			Camera::add_to_position(glm::vec3{ 0, 0, +MOVE_SPEED });
		}
		if (keyboardState[SDL_SCANCODE_S])
		{
			Camera::add_to_position(glm::vec3{ 0, 0, -MOVE_SPEED });
		}
		if (keyboardState[SDL_SCANCODE_Q])
		{
			Camera::add_to_position(glm::vec3{ 0, +MOVE_SPEED, 0});
		}
		if (keyboardState[SDL_SCANCODE_E])
		{
			Camera::add_to_position(glm::vec3{ 0, -MOVE_SPEED, 0 });
		}
	}

	// // ----- GETTERS ----- // //
	glm::mat4 FreeCamera::transform_to_cam(const glm::mat4& transform)
	{
		glm::vec3 position	= VectorUtils::extract_position(transform);
		glm::vec3 right		= glm::vec3{ transform[0][0], transform[0][1], transform[0][2] };
		glm::vec3 up		= glm::vec3{ transform[1][0], transform[1][1], transform[1][2] };
		glm::vec3 forward	= glm::vec3{ transform[2][0], transform[2][1], transform[2][2] };

		return glm::lookAt(position, position + forward, up);
	}

	glm::mat4 FreeCamera::cam_to_projection()
	{
		return glm::perspective(
			glm::radians(m_angle_of_view), 
			(GLfloat)Camera::dimensions().x / (GLfloat)Camera::dimensions().y, 
			Camera::clip_near(), 
			Camera::clip_far());
	}

	// // ----- SETTERS ----- // //
	void FreeCamera::set_angle_of_view(GLfloat angle)
	{
		m_angle_of_view = angle;
	}
}