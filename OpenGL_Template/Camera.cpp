#include "Camera.h"
#include <glm\gtx\transform.hpp>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL

const float Camera::moveSpeed      = 0.5f;
const float Camera::mouseMoveSpeed = 0.05f;
const float Camera::rotationSpeed  = 0.007f;

Camera::Camera() :

	viewDirection(-0.0, -1.0f, -1.0f),
	camRight(glm::vec3(-1.0f, 0.0f, 0.0f)),
	yAxis(glm::vec3(0.0f, 1.0f, 0.0f)),
	position(-0.5f, 4.0f,8.0f),
	camUp(cross(viewDirection, camRight)),
	lookTarget(/*glm::vec3(0.0f, 0.0f, 0.0f)*/position + viewDirection)

{
}

void  Camera::positionUpdate(const SDL_Scancode& newPosition) {
	switch (newPosition) {
	case SDL_SCANCODE_W:
		position += moveSpeed * viewDirection;
		break;
	case SDL_SCANCODE_S:
		position += -moveSpeed * viewDirection;
		break;
	case SDL_SCANCODE_A:
		position += -moveSpeed * camRight;
		break;
	case SDL_SCANCODE_D:
		position += moveSpeed * camRight;
		break;
	}
}

void Camera::moveRel(glm::vec3 mouseDelta) {

	glm::vec3 positionDelta =
		- camRight * mouseDelta.x
		+ camUp * mouseDelta.y
		+ viewDirection * mouseDelta.z;
	
	position += positionDelta;
	lookTarget += positionDelta;
	
}

void Camera::rotateRel(glm::vec2 rotateDelta) {

	pitch(rotateDelta.y);
	yaw(rotateDelta.x);

	if (rotateDelta.y != 0) {
		std::cout << "rotateDelta,     y: " << rotateDelta.y << "\n";
		std::cout << "rotateDelta, sin y: " << sin(rotateDelta.y) << "\n";
		std::cout << "viewDirection     : (" << viewDirection.x << ", " << viewDirection.y << ", " << viewDirection.z << ")\n";
		std::cout << "strafeDirection   : (" << camRight.x << ", " << camRight.y << ", " << camRight.z << ")\n";
		std::cout << "camUpDirection    : (" << camUp.x << ", " << camUp.y << ", " << camUp.z << ")\n";
		std::cout << "position          : (" << position.x << ", " << position.y << ", " << position.z << ")\n";
		std::cout << "lookTarget        : (" << lookTarget.x << ", " << lookTarget.y << ", " << lookTarget.z << ")\n";
		std::cout << "----------------------------\n";
	}
}

void Camera::pitch(std::float_t theta) {

	glm::mat3 rotator;
	rotator = (glm::mat3)glm::rotate(theta, camRight);
	position -= lookTarget;
	position = rotator * position;
	position += lookTarget;
	camUp = cross(camRight, position - lookTarget);
}

void Camera::yaw(std::float_t theta) {
	glm::mat3 rotator = (glm::mat3)glm::rotate(-theta, yAxis);
	position -= lookTarget;
	position = rotator * position;
	position += lookTarget;

	camRight = rotator * camRight;
	camUp = cross(camRight, position - lookTarget);
	viewDirection = cross(camRight, camUp);
}

void Camera::scrollUpdate(const float scrollAmount) {
	position += scrollAmount * viewDirection;
}

glm::mat4 Camera::getWorldToViewMatrix() const {
	return glm::lookAt(position, lookTarget, camUp);
}

glm::vec3 Camera::getPosition() const {
	return position;
}