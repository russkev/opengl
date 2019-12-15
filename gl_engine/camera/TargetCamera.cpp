#include "TargetCamera.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
namespace gl_engine
{
	const glm::vec3 TargetCamera::UP_AXIS = { 0.0f, 1.0f, 0.0f };

	const float TargetCamera::MOVE_SPEED = 0.5f;
	const float TargetCamera::MOUSE_MOVE_SPEED = 0.05f;
	const float TargetCamera::ROTATION_SPEED = 0.007f;

	TargetCamera::TargetCamera()
		//Camera()
	{}

	// // ----- CAMERA MOVEMENT ----- // //
	void TargetCamera::update()
	{
		static const auto cMoveSpeed = glm::vec3(0.02f, 0.01f, 0.1f);
		static const auto cRotateSpeed = glm::vec2(0.01f, 0.01f);

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
			focus(m_focusTarget);
		}
		moveRel(axisDelta * cMoveSpeed);
		rotateRel(rotateDelta * cRotateSpeed);
	}

	void TargetCamera::scrollUpdate(const float scrollAmount)
	{
		m_position += scrollAmount * m_viewDirection;
	}

	void TargetCamera::moveRel(const glm::vec3& mouseDelta)
	{
		glm::vec3 positionDelta =
			-m_camRight * mouseDelta.x
			+ m_camUp * mouseDelta.y
			+ m_viewDirection * mouseDelta.z;

		m_position += positionDelta;
		m_lookTarget += positionDelta;
	}

	void TargetCamera::rotateRel(const glm::vec2& rotateDelta)
	{
		glm::mat3 r_pitch = (glm::mat3)glm::rotate(glm::mat4(1.0f), rotateDelta.y, m_camRight);
		glm::mat3 r_yaw = (glm::mat3)glm::rotate(glm::mat4(1.0f), -rotateDelta.x, UP_AXIS);

		m_position -= m_lookTarget;
		m_position = r_pitch * r_yaw * m_position;
		m_position += m_lookTarget;

		m_viewDirection = m_position - m_lookTarget;
		m_camRight = r_yaw * m_camRight;
		m_camUp = cross(m_camRight, m_viewDirection);

		//printData(rotateDelta);
	}

	void TargetCamera::printData(const glm::vec2& rotateDelta)
	{
		if (rotateDelta.y != 0) {
			std::cout << "rotateDelta,     y: " << rotateDelta.y << "\n";
			std::cout << "rotateDelta, sin y: " << sin(rotateDelta.y) << "\n";
			std::cout << "viewDirection     : (" << m_viewDirection.x << ", " << m_viewDirection.y << ", " << m_viewDirection.z << ")\n";
			std::cout << "strafeDirection   : (" << m_camRight.x << ", " << m_camRight.y << ", " << m_camRight.z << ")\n";
			std::cout << "camUpDirection    : (" << m_camUp.x << ", " << m_camUp.y << ", " << m_camUp.z << ")\n";
			std::cout << "position          : (" << m_position.x << ", " << m_position.y << ", " << m_position.z << ")\n";
			std::cout << "lookTarget        : (" << m_lookTarget.x << ", " << m_lookTarget.y << ", " << m_lookTarget.z << ")\n";
			std::cout << "----------------------------\n";
		}
	}

	void TargetCamera::focus(const glm::vec3& focusTarget)
	{
		m_lookTarget = focusTarget;
	}
	// // ----- GETTERS ----- // //
	const glm::vec3 TargetCamera::position() const
	{
		return m_position;
	}

	glm::mat4 TargetCamera::worldToCam_matrix()
	{
		return glm::lookAt(m_position, m_lookTarget, m_camUp);
	}

	glm::mat4 TargetCamera::viewToProjectionMatrix()
	{
		return glm::perspective(glm::radians(m_angleOfView), (GLfloat)Camera::dimensions().x / (GLfloat)Camera::dimensions().y, Camera::clipNear(), Camera::clipFar());
	}

	// // ----- SETTERS ----- // //
	void TargetCamera::setPosition(glm::vec3 position)
	{
		m_position = position;
	}

	void TargetCamera::setViewDirection(glm::vec3 viewDirection)
	{
		m_viewDirection = viewDirection;
		setLookTarget();
		setCamUp();
	}

	void TargetCamera::setCamRight(glm::vec3 camRight)
	{
		m_camRight = camRight;
		setLookTarget();
		setCamUp();
	}

	void TargetCamera::setAngleOfView(GLfloat angleOfView)
	{
		m_angleOfView = angleOfView;
	}

	void TargetCamera::setFocusTarget(const glm::vec3& target)
	{
		m_focusTarget = target;
	}

	void TargetCamera::setLookTarget()
	{
		m_lookTarget = m_viewDirection + m_camRight;
	}

	void TargetCamera::setCamUp()
	{
		m_camUp = glm::cross(m_viewDirection, m_camRight);
	}
} // namespace gl_engine