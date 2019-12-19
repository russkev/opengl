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

		GLfloat m_angleOfView = 60.0f;

		// // ----- CONSTRUCTOR ----- // //
	public:
		FreeCamera();

		// // ----- CAMERA MOVEMENT ----- // //
		void update();

		// // ----- GETTERS ----- // //
		//const glm::vec3 position() const override;
		glm::mat4 transformToCam_matrix(const glm::mat4& transform) override;
		glm::mat4 camToProjection_matrix() override;

		// // ----- SETTERS ----- // //
		void setAngleOfView(GLfloat angle);
	};


} // namespace gl_engine
#endif