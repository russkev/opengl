#pragma once
#include <SDL.h>
#include <glm\matrix.hpp>

#define GLM_ENABLE_EXPERIMENTAL

class Camera
{
	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec3 up;
	glm::vec2 oldMousePosition;
	glm::vec3 strafeDirection;
	glm::vec3 camUpDirection;
	static const float moveSpeed;
	static const float mouseMoveSpeed;
	static const float rotationSpeed;
public:
	Camera();
	void mouseUpdate(const glm::vec2& newMousePosition, const bool altDown, const bool mouseDown);
	void positionUpdate(const SDL_Scancode& newPosition);
	void scrollUpdate(const float scrollAmount);
	glm::mat4 getWorldToViewMatrix() const;
};

