#include "Camera.h"
#include <glm\gtx\transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL

Camera::Camera() :
	viewDirection(0.0f, 0.0f, -1.0f),
	up(0.0f, 1.0f, 0.0f)
{
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition) {
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	viewDirection = glm::mat3(glm::rotate(mouseDelta.x*0.007f, up)) * viewDirection;
	oldMousePosition = newMousePosition;
}

glm::mat4 Camera::getWorldToViewMatrix() const {
	return glm::lookAt(position, position + viewDirection, up);
}


