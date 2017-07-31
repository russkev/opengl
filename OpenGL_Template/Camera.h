#pragma once
#include <SDL.h>
#include <glm\matrix.hpp>

#define GLM_ENABLE_EXPERIMENTAL

class Camera
{
	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec3 lookTarget;
	glm::vec3 yAxis;
	glm::vec2 oldMousePosition;
	glm::vec3 camRight;
	glm::vec3 camUp;
	static const float moveSpeed;
	static const float mouseMoveSpeed;
	static const float rotationSpeed;
public:
	Camera();
	void positionUpdate(const SDL_Scancode& newPosition);
	void scrollUpdate(const float scrollAmount);
	void moveRel(const glm::vec3& moveDelta);
	void rotateRel(const glm::vec2& rotateDelta);
	void printData(const glm::vec2& rotateDelta);
	glm::mat4 getWorldToViewMatrix() const;
	glm::vec3 getPosition() const;
};

