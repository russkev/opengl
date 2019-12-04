#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <glm/matrix.hpp>

#define GLM_ENABLE_EXPERIMENTAL

struct Camera
{

private:
	inline static const glm::vec3 UP_AXIS = { 0.0f, 1.0f, 0.0f };

	// // ----- MEMBER VARIABLES ----- // //
	glm::vec3 m_position = { 0.0f, 8.0f, 8.0f };
	glm::vec3 m_viewDirection = { 0.0f, -1.0f, -1.0f };
	glm::vec3 m_camRight = { -1.0f, 0.0f, 0.0f };

	glm::vec3 m_lookTarget = m_viewDirection + m_camRight;
	glm::vec3 m_camUp = glm::cross(m_viewDirection, m_camRight);

	glm::vec2 m_oldMousePosition;

	glm::ivec2 m_dimensions = { 1024, 576 };
	GLfloat m_clipNear		= 0.1f;
	GLfloat m_clipFar		= 100.0f;
	GLfloat m_angleOfView	= 60.0f; // In degrees

	static const float m_moveSpeed;
	static const float m_mouseMoveSpeed;
	static const float m_rotationSpeed;
public:
	// // ----- CONSTRUCTOR ----- // //
	Camera();
	
	// // ----- CAMERA MOVEMENT ----- // //
	void update();
	void positionUpdate(const SDL_Scancode& newPosition);
	void scrollUpdate(const float scrollAmount);
	void moveRel(const glm::vec3& moveDelta);
	void rotateRel(const glm::vec2& rotateDelta);
	void printData(const glm::vec2& rotateDelta);
	void focus(const glm::mat4& wldMatrix);

	glm::vec3 getPosition() const;

	// // ----- GETTERS ----- // //
	//glm::vec3 m_position = { 0.0f, 8.0f, 8.0f };
	//glm::vec3 m_viewDirection = { 0.0f, -1.0f, -1.0f };
	//glm::vec3 m_camRight = { -1.0f, 0.0f, 0.0f };

	//glm::vec3 m_lookTarget = m_viewDirection + m_camRight;
	//glm::vec3 m_camUp = glm::cross(m_viewDirection, m_camRight);

	//glm::vec2 m_oldMousePosition;

	const glm::ivec2& dimensions() const;
	//GLfloat m_clipNear = 0.1f;
	//GLfloat m_clipFar = 100.0f;
	//GLfloat m_angleOfView = 60.0f; // In degrees
	glm::mat4 worldToViewMatrix();
	glm::mat4 viewToProjectionMatrix();
	glm::mat4 worldToProjectionMatrix();


	// // ----- SETTERS ----- // //
	void setPosition(glm::vec3);
	void setViewDirection(glm::vec3);
	void setCamRight(glm::vec3);

	void setDimensions(glm::ivec2);
	void setClipNear(GLfloat);
	void setClipFar(GLfloat);
	void setAngleOfView(GLfloat);

private:
	void setLookTarget();
	void setCamUp();

};

