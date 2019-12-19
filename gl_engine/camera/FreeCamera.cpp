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
			Camera::addPosition(glm::vec3{ +MOVE_SPEED, 0, 0 });
			//m_position = glm::vec3{ m_position.x + MOVE_SPEED, m_position.y, m_position.z };
		}
		if (keyboardState[SDL_SCANCODE_D])
		{
			Camera::addPosition(glm::vec3{ -MOVE_SPEED, 0, 0 });
			//m_position = glm::vec3{ m_position.x - MOVE_SPEED, m_position.y, m_position.z };
		}
		if (keyboardState[SDL_SCANCODE_W])
		{
			Camera::addPosition(glm::vec3{ 0, 0, +MOVE_SPEED });
			//m_position = glm::vec3{ m_position.x, m_position.y, m_position.z + MOVE_SPEED };
		}
		if (keyboardState[SDL_SCANCODE_S])
		{
			Camera::addPosition(glm::vec3{ 0, 0, -MOVE_SPEED });
			//m_position = glm::vec3{ m_position.x, m_position.y, m_position.z - MOVE_SPEED };
		}
		if (keyboardState[SDL_SCANCODE_Q])
		{
			Camera::addPosition(glm::vec3{ 0, +MOVE_SPEED, 0});
			//m_position = glm::vec3{ m_position.x, m_position.y + MOVE_SPEED, m_position.z };
		}
		if (keyboardState[SDL_SCANCODE_E])
		{
			Camera::addPosition(glm::vec3{ 0, -MOVE_SPEED, 0 });
			//m_position = glm::vec3{ m_position.x, m_position.y - MOVE_SPEED, m_position.z };
		}
	}

	// // ----- GETTERS ----- // //
	//const glm::vec3 FreeCamera::position() const
	//{
	//	return m_position;
	//}

	glm::mat4 FreeCamera::transformToCam_matrix(const glm::mat4& transform)
	{
		//glm::mat4 out_matrix = transform;
		//glm::mat4 flip_matrix{ 1.0f };
		//flip_matrix[0][0] = -1;
		//flip_matrix[2][2] = -1;

		//glm::vec3 position = -VectorUtils::extract_position(out_matrix);
		//VectorUtils::set_position(out_matrix, position);

		//return flip_matrix * out_matrix;



		//return *Camera::transform();
		//return glm::inverse(transform);
		//return glm::translate(glm::mat4(1.0f), m_position);		

		glm::vec3 position	= VectorUtils::extract_position(transform);
		glm::vec3 right		= glm::vec3{ transform[0][0], transform[0][1], transform[0][2] };
		glm::vec3 up		= glm::vec3{ transform[1][0], transform[1][1], transform[1][2] };
		glm::vec3 forward	= glm::vec3{ transform[2][0], transform[2][1], transform[2][2] };

		return glm::lookAt(position, position + forward, up);



	}

	glm::mat4 FreeCamera::camToProjection_matrix()
	{
		return glm::perspective(
			glm::radians(m_angleOfView), 
			(GLfloat)Camera::dimensions().x / (GLfloat)Camera::dimensions().y, 
			Camera::clipNear(), 
			Camera::clipFar());
	}

	// // ----- SETTERS ----- // //
	void FreeCamera::setAngleOfView(GLfloat angle)
	{
		m_angleOfView = angle;
	}
}