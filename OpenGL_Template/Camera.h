#pragma once
#include <glm\matrix.hpp>

#define GLM_ENABLE_EXPERIMENTAL

class Camera
{
	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec3 up;
	glm::vec2 oldMousePosition;
public:
	Camera();
	void mouseUpdate(const glm::vec2& newMousePosition);
	glm::mat4 getWorldToViewMatrix() const;
};

