#ifndef GL_ENGINE_CAMERA_FREE_CAMERA_H
#define GL_ENGINE_CAMERA_FREE_CAMERA_H

#include "Camera.h"

namespace gl_engine
{
	/*

		Free camera. Not targeted. Orientation is based on parents transforms only

	*/
	struct FreeCamera : public Camera
	{
	private:
		static const float MOVE_SPEED;

		GLfloat m_angleOfView = 1.0f;
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };

		// // ----- CONSTRUCTOR ----- // //
	public:
		FreeCamera();

		// // ----- CAMERA MOVEMENT ----- // //
		void update();

		// // ----- GETTERS ----- // //
		glm::mat4 worldToCam_matrix() override;
		glm::mat4 viewToProjectionMatrix() override;

		// // ----- SETTERS ----- // //
		void setAngleOfView(GLfloat angle);
	};


} // namespace gl_engine
#endif