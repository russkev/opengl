#ifndef GL_ENGINE_CAMERA_ORTHO_CAMERA_H
#define GL_ENGINE_CAMERA_ORTHO_CAMERA_H

#include "Camera.h"

namespace gl_engine
{
	/*

		Orthographic camera. No perspective.

	*/
	struct OrthoCamera : public Camera
	{
		// // ----- CONSTRUCTORS ----- // //
	public:
		OrthoCamera();
		OrthoCamera(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top);

		// // ----- CAMERA MOVEMENT ----- // //
		void update();

		// // ----- GETTERS ----- // //
		const glm::mat4 transform_to_cam(const glm::mat4& transform) const override;
		const glm::mat4 cam_to_projection() const override;


		// // ----- SETTERS ----- // //
		void set_sides(const GLfloat left, const GLfloat right, const GLfloat bottom, const GLfloat top);
		void set_left(const GLfloat left);
		void set_right(const GLfloat right);
		void set_bottom(const GLfloat bottom);
		void set_top(const GLfloat top);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		GLfloat m_left = -10.0f;
		GLfloat m_right = 10.0f;
		GLfloat m_bottom = -10.0f;
		GLfloat m_top = 10.0f;
	};
} // namespace gl_engine
#endif