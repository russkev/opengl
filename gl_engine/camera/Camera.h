#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <glm/matrix.hpp>

#define GLM_ENABLE_EXPERIMENTAL

struct Camera
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

	glm::uvec2 m_dimensions = { 800u, 600u };
	GLfloat m_clipNear		= 0.1f;
	GLfloat m_clipFar		= 10.0f;
	GLfloat m_angleOfView	= 50.0f; // In degrees


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

	virtual const glm::vec3 position() const;

	// // ----- GETTERS ----- // //
	const glm::uvec2& dimensions() const;
	//GLfloat m_clipNear = 0.1f;
	//GLfloat m_clipFar = 100.0f;
	//GLfloat m_angleOfView = 60.0f; // In degrees
	virtual glm::mat4 worldToCam_matrix();
	glm::mat4 viewToProjectionMatrix();
	virtual glm::mat4 worldToProjection_matrix();


	// // ----- SETTERS ----- // //
	void setPosition(glm::vec3);
	void setViewDirection(glm::vec3);
	void setCamRight(glm::vec3);

	void setDimensions(glm::uvec2);
	void setClipNear(GLfloat);
	void setClipFar(GLfloat);
	void setAngleOfView(GLfloat);

private:
	void setLookTarget();
	void setCamUp();

};

