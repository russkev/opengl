#include "Camera.h"
#include <glm\gtx\transform.hpp>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL

const float Camera::m_moveSpeed      = 0.5f;
const float Camera::m_mouseMoveSpeed = 0.05f;
const float Camera::m_rotationSpeed  = 0.007f;

Camera::Camera() :

	m_viewDirection(-0.0, -1.0f, -1.0f),
	m_camRight(glm::vec3(-1.0f, 0.0f, 0.0f)),
	m_yAxis(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_position(-0.5f, 4.0f,8.0f),
	m_camUp(cross(m_viewDirection, m_camRight)),
	m_lookTarget(/*glm::vec3(0.0f, 0.0f, 0.0f)*/m_position + m_viewDirection)

{
}

void  Camera::positionUpdate(const SDL_Scancode& newPosition) {
	switch (newPosition) {
	case SDL_SCANCODE_W:
		m_position += m_moveSpeed * m_viewDirection;
		break;
	case SDL_SCANCODE_S:
		m_position += -m_moveSpeed * m_viewDirection;
		break;
	case SDL_SCANCODE_A:
		m_position += -m_moveSpeed * m_camRight;
		break;
	case SDL_SCANCODE_D:
		m_position += m_moveSpeed * m_camRight;
		break;
	}
}

void Camera::moveRel(const glm::vec3& mouseDelta) {

	glm::vec3 positionDelta =
		- m_camRight * mouseDelta.x
		+ m_camUp * mouseDelta.y
		+ m_viewDirection * mouseDelta.z;
	
	m_position += positionDelta;
	m_lookTarget += positionDelta;
	
}

void Camera::rotateRel(const glm::vec2& rotateDelta) {


	glm::mat3 r_pitch = (glm::mat3)glm::rotate( rotateDelta.y, m_camRight);
	glm::mat3 r_yaw   = (glm::mat3)glm::rotate(-rotateDelta.x, m_yAxis);
	m_position -= m_lookTarget;
	m_position = r_pitch * r_yaw * m_position;
	m_position += m_lookTarget;

	m_viewDirection   = m_position - m_lookTarget;
	m_camRight		= r_yaw * m_camRight;
	m_camUp			= cross(m_camRight, m_viewDirection);

	//printData(rotateDelta);
}

void Camera::printData(const glm::vec2& rotateDelta) {
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

void Camera::focus(const glm::mat4& wldMatrix) {
	m_lookTarget = {wldMatrix[3][0], wldMatrix[3][1], wldMatrix[3][2]};
}


void Camera::scrollUpdate(const float scrollAmount) {
	m_position += scrollAmount * m_viewDirection;
}

glm::mat4 Camera::getWorldToViewMatrix() const {
	return glm::lookAt(m_position, m_lookTarget, m_camUp);
}

glm::vec3 Camera::getPosition() const {
	return m_position;
}