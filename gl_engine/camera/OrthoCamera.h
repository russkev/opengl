#ifndef GL_ENGINE_CAMERA_ORTHO_CAMERA_H
#define GL_ENGINE_CAMERA_ORTHO_CAMERA_H

#include "Camera.h"

struct OrthoCamera : public Camera
{
	// // ----- MEMBER VARIABLES ----- // //
private:
	GLfloat m_left		= -10.0f;
	GLfloat m_right		=  10.0f;
	GLfloat m_bottom	= -10.0f;
	GLfloat m_top		=  10.0f;

	// // ----- CONSTRUCTOR ----- // //
public:
	OrthoCamera();
	OrthoCamera(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top);

	// // ----- CAMERA MOVEMENT ----- // //
	void update();

	// // ----- GETTERS ----- // //
	glm::mat4 worldToCam_matrix() override;
	glm::mat4 viewToProjectionMatrix() override;

	// // ----- SETTERS ----- // //
	void setSides(const GLfloat left, const GLfloat right, const GLfloat bottom, const GLfloat top);
	void setLeft(const GLfloat left);
	void setRight(const GLfloat right);
	void setBottom(const GLfloat bottom);
	void setTop(const GLfloat top);
};

#endif