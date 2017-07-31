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
	void moveRel(glm::vec3 moveDelta);
	void rotateRel(glm::vec2 rotateDelta);
	void pitch(std::float_t theta);
	void yaw(std::float_t theta);
	glm::mat4 getWorldToViewMatrix() const;
	glm::vec3 getPosition() const;
};

