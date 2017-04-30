#include "Camera.h"
#include <glm\gtx\transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL

const float Camera::moveSpeed     = 0.5;
const float Camera::rotationSpeed = 0.007f;

Camera::Camera() :
	viewDirection(0.0f, 0.0f, -1.0f),
	up(0.0f, 1.0f, 0.0f),
	position(4.0, 4.0, 3.0)
{
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition) {
	const glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) < 100.0f) {
		const float rotationSpeed = 0.007f;
		strafeDirection = glm::cross(viewDirection, up);
		glm::mat3 rotator =
			glm::mat3(glm::rotate(-mouseDelta.x*rotationSpeed, up)) *
			glm::mat3(glm::rotate(-mouseDelta.y*rotationSpeed, strafeDirection));
		viewDirection = rotator * viewDirection;
	}
	oldMousePosition = newMousePosition;
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
		position += -moveSpeed * strafeDirection;
		break;
	case SDL_SCANCODE_D:
		position += moveSpeed * strafeDirection;
		break;
	}
}


glm::mat4 Camera::getWorldToViewMatrix() const {
	return glm::lookAt(position, position + viewDirection, up);
}


