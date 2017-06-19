#include "Camera.h"
#include <glm\gtx\transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL

const float Camera::moveSpeed      = 0.5f;
const float Camera::mouseMoveSpeed = 0.05f;
const float Camera::rotationSpeed  = 0.007f;

Camera::Camera() :
	viewDirection(-0.0, -1.0f, -1.0f),
	up(0.0f, 1.0f, 0.0f),
	position(-0.5f, 4.0f, 2.0f)
{
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition, const bool altDown, const bool mouseDown) {
	glm::vec2 mouseDelta = glm::vec2(0, 0);
	if (mouseDown) {
		mouseDelta = newMousePosition - oldMousePosition;
	}
	strafeDirection = glm::cross(viewDirection, up);
	camUpDirection = glm::cross(viewDirection, strafeDirection);
	if (altDown) {
		position += mouseMoveSpeed * mouseDelta.x * strafeDirection;
		position += mouseMoveSpeed * mouseDelta.y * camUpDirection;
	}
	else {
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

void Camera::scrollUpdate(const float scrollAmount) {
	position += scrollAmount * viewDirection;
}


glm::mat4 Camera::getWorldToViewMatrix() const {
	return glm::lookAt(position, position + viewDirection, up);
}


