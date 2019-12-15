#include "OrbitCamera.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 OrbitCamera::UP_AXIS = { 0.0f, 1.0f, 0.0f };

const float OrbitCamera::MOVE_SPEED = 0.5f;
const float OrbitCamera::MOUSE_MOVE_SPEED = 0.05f;
const float OrbitCamera::ROTATION_SPEED = 0.007f;

OrbitCamera::OrbitCamera()
	//Camera()
{}

// // ----- CAMERA MOVEMENT ----- // //
void OrbitCamera::update()
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
		focus(glm::mat4(1.0f));
	}
	moveRel(axisDelta * cMoveSpeed);
	rotateRel(rotateDelta * cRotateSpeed);
}

void OrbitCamera::scrollUpdate(const float scrollAmount)
{
	m_position += scrollAmount * m_viewDirection;
}

void OrbitCamera::moveRel(const glm::vec3& mouseDelta)
{
	glm::vec3 positionDelta =
		-m_camRight * mouseDelta.x
		+ m_camUp * mouseDelta.y
		+ m_viewDirection * mouseDelta.z;

	m_position += positionDelta;
	m_lookTarget += positionDelta;
}

void OrbitCamera::rotateRel(const glm::vec2& rotateDelta)
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

void OrbitCamera::printData(const glm::vec2& rotateDelta)
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

void OrbitCamera::focus(const glm::mat4& wldMatrix)
{
	m_lookTarget = { wldMatrix[3][0], wldMatrix[3][1], wldMatrix[3][2] };
}
// // ----- GETTERS ----- // //
const glm::vec3 OrbitCamera::position() const
{
	return m_position;
}

glm::mat4 OrbitCamera::worldToCam_matrix()
{
	return glm::lookAt(m_position, m_lookTarget, m_camUp);
}

glm::mat4 OrbitCamera::viewToProjectionMatrix()
{
	return glm::perspective(glm::radians(m_angleOfView), (GLfloat)Camera::dimensions().x / (GLfloat)Camera::dimensions().y, Camera::clipNear(), Camera::clipFar());
}

// // ----- SETTERS ----- // //
void OrbitCamera::setPosition(glm::vec3 position)
{
	m_position = position;
}

void OrbitCamera::setViewDirection(glm::vec3 viewDirection)
{
	m_viewDirection = viewDirection;
	setLookTarget();
	setCamUp();
}

void OrbitCamera::setCamRight(glm::vec3 camRight)
{
	m_camRight = camRight;
	setLookTarget();
	setCamUp();
}

void OrbitCamera::setAngleOfView(GLfloat angleOfView)
{
	m_angleOfView = angleOfView;
}

void OrbitCamera::setLookTarget()
{
	m_lookTarget = m_viewDirection + m_camRight;
}

void OrbitCamera::setCamUp()
{
	m_camUp = glm::cross(m_viewDirection, m_camRight);
}
