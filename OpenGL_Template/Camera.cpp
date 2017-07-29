#include "Camera.h"
#include <glm\gtx\transform.hpp>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL

const float Camera::moveSpeed      = 0.5f;
const float Camera::mouseMoveSpeed = 0.05f;
const float Camera::rotationSpeed  = 0.007f;

Camera::Camera() :

	viewDirection(-0.0, -1.0f, -1.0f),
	strafeDirection(glm::vec3(1.0f, 0.0f, 0.0f)),
	yAxis(glm::vec3(0.0f, 1.0f, 0.0f)),
	position(-0.5f, 4.0f, 2.0f)
{
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition, const bool altDown, const bool mouseDown) {
	glm::vec2 mouseDelta = glm::vec2(0, 0);
	if (mouseDown) {
		mouseDelta = newMousePosition - oldMousePosition;
	}
	//strafeDirection = glm::cross(viewDirection, yAxis);
	//camUpDirection = glm::cross(viewDirection, strafeDirection);
	if (altDown) {
		position += mouseMoveSpeed * mouseDelta.x * strafeDirection;
		position += mouseMoveSpeed * mouseDelta.y * camUpDirection;
	}
	else {
		glm::mat3 rotator =
			glm::mat3(glm::rotate(-mouseDelta.x*rotationSpeed, yAxis)) *
			glm::mat3(glm::rotate(-mouseDelta.y*rotationSpeed, strafeDirection));
		//viewDirection = rotator * viewDirection;
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

void Camera::moveRel(glm::vec3 positionDelta) {
	//strafeDirection = glm::normalize(glm::cross(viewDirection, yAxis));
	//if (up != camUpDirection) {
	//	camUpDirection 
	//}
	//camUpDirection  = glm::normalize(glm::cross(viewDirection, strafeDirection));
	//up = camUpDirection;
	//std::cout << "View Direction: " << viewDirection.x << ", " << viewDirection.y << ", " << viewDirection.z << "\n";
	//std::cout << "Cam Up Direction: " << camUpDirection.x << ", " << camUpDirection.y << ", " << camUpDirection.z << "\n";
	//std::cout << "Strafe Direction: " << strafeDirection.x << ", " << strafeDirection.y << ", " << strafeDirection.z << "\n";
	position += strafeDirection * positionDelta.x;
	position -= camUpDirection  * positionDelta.y;
	position += viewDirection   * positionDelta.z;
	
}

void Camera::rotateRel(glm::vec2 rotateDelta) {

	pitch(rotateDelta.y);

	//strafeDirection = glm::cross(viewDirection, up);
	//camUpDirection = glm::normalize(glm::cross(viewDirection, strafeDirection));
	//glm::mat3 rotator =
	//	/*glm::mat3(glm::rotate(-rotateDelta.x, up)) **/
	//	glm::mat3(glm::rotate(-rotateDelta.y, strafeDirection));
	//viewDirection = rotator * viewDirection;
	//viewDirection.x += cos(rotateDelta.y) * cos(rotateDelta.x);
	//viewDirection.y += sin(rotateDelta.y);
	if (rotateDelta.y != 0) {
		std::cout << "rotateDelta,     y: " << rotateDelta.y << "\n";
		std::cout << "rotateDelta, sin y: " << sin(rotateDelta.y) << "\n";
		std::cout << "viewDirection     : (" << viewDirection.x << ", " << viewDirection.y << ", " << viewDirection.z << ")\n";
		std::cout << "strafeDirection   : (" << strafeDirection.x << ", " << strafeDirection.y << ", " << strafeDirection.z << ")\n";
		std::cout << "----------------------------\n";
	}
	//viewDirection.z += cos(rotateDelta.y) * sin(rotateDelta.x);
}

void Camera::pitch(std::float_t theta) {

	//glm::mat3 rotator;
	//if (sin(theta) < 0) {
	//	yAxis = glm::vec3(0, 1, 0);
	//}
	//else {
	//	yAxis = glm::vec3(0, -1, 0);
	//}
	//rotator = (glm::mat3)glm::rotate(theta, strafeDirection);
	//viewDirection = rotator * viewDirection;
}

void Camera::yaw(std::float_t theta) {
	return;
}

//glm::vec3 Camera::rotateQuaternion(std::float_t angle, glm::vec3 axis) {
//	float sinHalfAngle = (float)sin(angle / 2.0f);
//	float cosHalfAngle = (float)cos(angle / 2.0f);
//
//	std::float_t rx = axis.x*sinHalfAngle;
//	std::float_t ry = axis.y*sinHalfAngle;
//	std::float_t rz = axis.z*sinHalfAngle;
//	std::float_t rw = cosHalfAngle;
//
//	glm::vec4 quaternion = { rw, rx, ry, rz };
//
//	glm::vec4 outRotation = quaternion * glm::vec4{ 0, viewDirection };
//}

void Camera::scrollUpdate(const float scrollAmount) {
	position += scrollAmount * viewDirection;
}

glm::mat4 Camera::getWorldToViewMatrix() const {
	return glm::lookAt(position, position + viewDirection, yAxis);
}

glm::vec3 Camera::getPosition() const {
	return position;
}