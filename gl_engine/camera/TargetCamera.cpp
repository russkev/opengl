#include "TargetCamera.h"

#include "../utils/VectorUtils.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace gl_engine
{
	// // ----- STATICS ----- // //
	const glm::vec3 TargetCamera::UP_AXIS = { 0.0f, 1.0f, 0.0f };
	const float TargetCamera::MOVE_SPEED = 0.005f;
	const float TargetCamera::ROTATION_SPEED = 0.007f;

	// // ----- CONSTRUCTORS ----- // //
	TargetCamera::TargetCamera()
	{}

	// // ----- CAMERA MOVEMENT ----- // //
	void TargetCamera::update()
	{

		//static const auto cMoveSpeed = glm::vec3(0.02f, 0.01f, 0.1f);
		//static const auto cRotateSpeed = glm::vec2(0.01f, 0.01f);

		auto const keyboardState = SDL_GetKeyboardState(nullptr);
		auto mouseDelta = glm::ivec2();
		auto mouseButton = SDL_GetRelativeMouseState(&mouseDelta.x, &mouseDelta.y);
		auto axisDelta = glm::vec3();
		auto rotateDelta = glm::vec2();

		if (mouseButton & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			rotateDelta = (glm::vec2)mouseDelta;
		}
		if (mouseButton & SDL_BUTTON(SDL_BUTTON_MIDDLE))
		{
			axisDelta.x = -(float)mouseDelta.x;
			axisDelta.y = (float)mouseDelta.y;
		}

		if (mouseButton & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			axisDelta.z = (float)mouseDelta.y;
		}
		if (keyboardState[SDL_SCANCODE_F] || keyboardState[SDL_SCANCODE_Z])
		{
			focus(m_focus_target);
		}
		move_relative(axisDelta * MOVE_SPEED);
		rotate_relative(rotateDelta * ROTATION_SPEED);
	}

	void TargetCamera::move_relative(const glm::vec3& mouse_delta)
	{
		glm::vec3 positionDelta =
			-m_cam_right * mouse_delta.x
			+ m_cam_up * mouse_delta.y
			+ m_view_direction * mouse_delta.z;

		Camera::add_to_position(positionDelta);
		m_look_target += positionDelta;
	}

	void TargetCamera::rotate_relative(const glm::vec2& rotateDelta)
	{
		glm::mat3 pitch = (glm::mat3)glm::rotate(glm::mat4(1.0f), rotateDelta.y, m_cam_right);
		glm::mat3 yaw = (glm::mat3)glm::rotate(glm::mat4(1.0f), -rotateDelta.x, UP_AXIS);

		Camera::add_to_position(-m_look_target);
		Camera::set_position(pitch * yaw * Camera::position());
		Camera::add_to_position(m_look_target);

		m_view_direction = VectorUtils::extract_position(*Camera::transform());
		m_cam_right = yaw * m_cam_right;
		m_cam_up = cross(m_cam_right, m_view_direction);

		//print_data(rotate_delta);
	}

	void TargetCamera::print_data(const glm::vec2& rotate_delta)
	{
		glm::vec3 position = VectorUtils::extract_position(*Camera::transform());
		if (rotate_delta.y != 0) {
			std::cout << "rotate_delta,     y: " << rotate_delta.y << "\n";
			std::cout << "rotate_delta, sin y: " << sin(rotate_delta.y) << "\n";
			std::cout << "view_direction     : (" << m_view_direction.x << ", " << m_view_direction.y << ", " << m_view_direction.z << ")\n";
			std::cout << "strafeDirection   : (" << m_cam_right.x << ", " << m_cam_right.y << ", " << m_cam_right.z << ")\n";
			std::cout << "camUpDirection    : (" << m_cam_up.x << ", " << m_cam_up.y << ", " << m_cam_up.z << ")\n";
			std::cout << "position          : (" << position.x << ", " << position.y << ", " << position.z << ")\n";
			std::cout << "lookTarget        : (" << m_look_target.x << ", " << m_look_target.y << ", " << m_look_target.z << ")\n";
			std::cout << "----------------------------\n";
		}
	}

	void TargetCamera::focus(const glm::vec3& focus_target)
	{
		m_look_target = focus_target;
	}

	// // ----- GETTERS ----- // //

	glm::mat4 TargetCamera::transform_to_cam(const glm::mat4& transform)
	{
		glm::vec3 position = VectorUtils::extract_position(transform);
		return glm::lookAt(position, m_look_target, m_cam_up);
	}

	glm::mat4 TargetCamera::cam_to_projection()
	{
		return glm::perspective(
			glm::radians(m_angle_of_view), 
			(GLfloat)Camera::dimensions().x / (GLfloat)Camera::dimensions().y, 
			Camera::clip_near(), 
			Camera::clip_far());
	}

	// // ----- SETTERS ----- // //
	void TargetCamera::set_view_direction(glm::vec3 view_direction)
	{
		m_view_direction = view_direction;
		set_look_target();
		set_cam_up();
	}

	void TargetCamera::set_cam_right(glm::vec3 cam_right)
	{
		m_cam_right = cam_right;
		set_look_target();
		set_cam_up();
	}

	void TargetCamera::set_angle_of_view(GLfloat angle_of_view)
	{
		m_angle_of_view = angle_of_view;
	}

	void TargetCamera::set_focus_target(const glm::vec3& target)
	{
		m_focus_target = target;
	}

	void TargetCamera::set_look_target()
	{
		m_look_target = m_view_direction + m_cam_right;
	}

	void TargetCamera::set_cam_up()
	{
		m_cam_up = glm::cross(m_view_direction, m_cam_right);
	}
} // namespace gl_engine