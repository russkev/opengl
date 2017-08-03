#pragma once
#include <SDL.h>
#include <glm\matrix.hpp>

#define GLM_ENABLE_EXPERIMENTAL

class Camera
{
	glm::vec3 m_position;
	glm::vec3 m_viewDirection;
	glm::vec3 m_lookTarget;
	glm::vec3 m_yAxis;
	glm::vec2 m_oldMousePosition;
	glm::vec3 m_camRight;
	glm::vec3 m_camUp;
	static const float m_moveSpeed;
	static const float m_mouseMoveSpeed;
	static const float m_rotationSpeed;
public:
	Camera();
	void positionUpdate(const SDL_Scancode& newPosition);
	void scrollUpdate(const float scrollAmount);
	void moveRel(const glm::vec3& moveDelta);
	void rotateRel(const glm::vec2& rotateDelta);
	void printData(const glm::vec2& rotateDelta);
	void focus(const glm::mat4& wldMatrix);
	glm::mat4 getWorldToViewMatrix() const;
	glm::vec3 getPosition() const;
};

