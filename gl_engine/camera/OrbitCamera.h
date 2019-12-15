#ifndef GL_ENGINE_CAMERA_ORBIT_CAMERA_H
#define GL_ENGINE_CAMERA_ORBIT_CAMERA_H

#include "Camera.h"

struct OrbitCamera : public Camera
{
private:
	static const glm::vec3 UP_AXIS;
	static const float MOVE_SPEED;
	static const float MOUSE_MOVE_SPEED;
	static const float ROTATION_SPEED;

	// // ----- MEMBER VARIABLES ----- // //
	glm::vec3 m_position = { 0.0f, 8.0f, 8.0f };
	glm::vec3 m_viewDirection = { 0.0f, -1.0f, -1.0f };
	glm::vec3 m_camRight = { -1.0f, 0.0f, 0.0f };

	glm::vec3 m_lookTarget = m_viewDirection + m_camRight;
	glm::vec3 m_camUp = glm::cross(m_viewDirection, m_camRight);

	glm::vec2 m_oldMousePosition;

	GLfloat m_angleOfView = 50.0f; // In degrees

	// // ----- CONSTRUCTOR ----- // //
public:
	OrbitCamera();

	// // ----- CAMERA MOVEMENT ----- // //
	void update();
	//void positionUpdate(const SDL_Scancode& newPosition);
	void scrollUpdate(const float scrollAmount);
	void moveRel(const glm::vec3& moveDelta);
	void rotateRel(const glm::vec2& rotateDelta);
	void printData(const glm::vec2& rotateDelta);
	void focus(const glm::mat4& wldMatrix);

	// // ----- GETTERS ----- // //
	const glm::vec3 position() const override;
	glm::mat4 worldToCam_matrix() override;
	glm::mat4 viewToProjectionMatrix() override;

	// // ----- SETTERS ----- // //
	void setPosition(glm::vec3);
	void setViewDirection(glm::vec3);
	void setCamRight(glm::vec3);
	void setAngleOfView(GLfloat);

private:
	void setLookTarget();
	void setCamUp();
};

#endif