#ifndef GL_ENGINE_CAMERA_FREE_CAMERA_H
#define GL_ENGINE_CAMERA_FREE_CAMERA_H

#include "Camera.h"

namespace gl_engine_exe
{
	/*

		Free camera. Not targeted. Orientation is based on parents transforms only

	*/
	struct FreeCamera : public Camera
	{
		// // ----- CONSTANTS ----- // //
	private:
		static const float MOVE_SPEED;

		// // ----- CONSTRUCTOR ----- // //
	public:
		FreeCamera();

		// // ----- CAMERA MOVEMENT ----- // //
		void update();

		// // ----- GETTERS ----- // //
		const glm::mat4 transform_to_cam(const glm::mat4& transform) const override;
		const glm::mat4 cam_to_projection() const override;

		// // ----- SETTERS ----- // //
		void set_angle_of_view(GLfloat angle);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		GLfloat m_angle_of_view = 60.0f;
	};


} // namespace gl_engine
#endif