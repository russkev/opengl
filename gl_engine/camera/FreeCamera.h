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

		GLfloat m_angle_of_view = 60.0f;

		// // ----- CONSTRUCTOR ----- // //
	public:
		FreeCamera();

		// // ----- CAMERA MOVEMENT ----- // //
		void update();

		// // ----- GETTERS ----- // //
		glm::mat4 transform_to_cam(const glm::mat4& transform) override;
		glm::mat4 cam_to_projection() override;

		// // ----- SETTERS ----- // //
		void set_angle_of_view(GLfloat angle);
	};


} // namespace gl_engine
#endif