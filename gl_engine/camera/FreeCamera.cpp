#include "FreeCamera.h"

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
			m_position = glm::vec3{ m_position.x + MOVE_SPEED, m_position.y, m_position.z };
		}
		if (keyboardState[SDL_SCANCODE_D])
		{
			m_position = glm::vec3{ m_position.x - MOVE_SPEED, m_position.y, m_position.z };
		}
		if (keyboardState[SDL_SCANCODE_W])
		{
			m_position = glm::vec3{ m_position.x, m_position.y, m_position.z + MOVE_SPEED };
		}
		if (keyboardState[SDL_SCANCODE_S])
		{
			m_position = glm::vec3{ m_position.x, m_position.y, m_position.z - MOVE_SPEED };
		}
		if (keyboardState[SDL_SCANCODE_Q])
		{
			m_position = glm::vec3{ m_position.x, m_position.y + MOVE_SPEED, m_position.z };
		}
		if (keyboardState[SDL_SCANCODE_E])
		{
			m_position = glm::vec3{ m_position.x, m_position.y - MOVE_SPEED, m_position.z };
		}
	}

	// // ----- GETTERS ----- // //
	const glm::vec3 FreeCamera::position() const
	{
		return m_position;
	}

	glm::mat4 FreeCamera::worldToCam_matrix()
	{
		return glm::translate(glm::mat4(1.0f), m_position);		
	}

	glm::mat4 FreeCamera::viewToProjection_matrix()
	{
		return glm::perspective(
			glm::radians(m_angleOfView), 
			(GLfloat)Camera::dimensions().x / (GLfloat)Camera::dimensions().y, 
			Camera::clipNear(), 
			Camera::clipFar());
	}

	glm::mat4 FreeCamera::worldToProjection_matrix()
	{
		return viewToProjection_matrix() * worldToCam_matrix();
	}

	// // ----- SETTERS ----- // //
	void FreeCamera::setAngleOfView(GLfloat angle)
	{
		m_angleOfView = angle;
	}
}