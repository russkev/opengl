#ifndef GL_ENGINE_CAMERA_ORTHO_CAMERA_H
#define GL_ENGINE_CAMERA_ORTHO_CAMERA_H

#include "Camera.h"

namespace glen
{
	/*!

		Orthographic camera. No perspective.

	*/
	struct OrthoCamera : public Camera
	{
		// // ----- CONSTRUCTORS ----- // //
	public:
		/*!
		* Constructor
		*/
		OrthoCamera();

		/*!
		* Constructor
		* /param left coordinate of left vertical clipping plane
		* /param right coordinate of right vertical clipping plane
		* /param bottom coordinate of bottom horizontal clipping plane
		* /param top coordinate of top horizontal clipping plane
		*/
		OrthoCamera(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top);

		// // ----- CAMERA MOVEMENT ----- // //
		/*!
		* Update the camera position.
		* At the moment this does nothing and movement is entirely dependent on `CameraNode` movement.
		*/
		void update();

		// // ----- GETTERS ----- // //

		/*!
		* \returns the transformation matrix from \a input matrix to projection.
		*/
		const glm::mat4 transform_to_cam(const glm::mat4& transform) const override;

		/*!
		* \returns the transformation matrix from the camera to projection.
		* The projection matrix is the matrix required to transform the scene into a perspective view.
		*/
		const glm::mat4 cam_to_projection() const override;


		// // ----- SETTERS ----- // //
		/*!
		* Set vertical and horizontal clipping planes
		* /param left coordinate of left vertical clipping plane
		* /param right coordinate of right vertical clipping plane
		* /param bottom coordinate of bottom horizontal clipping plane
		* /param top coordinate of top horizontal clipping plane
		*/
		void set_sides(const GLfloat left, const GLfloat right, const GLfloat bottom, const GLfloat top);

		/*!
		* /param left coordinate of left vertical clipping plane
		*/
		void set_left(const GLfloat left);

		/*!
		* /param right coordinate of right vertical clipping plane
		*/
		void set_right(const GLfloat right);

		/*!
		* /param bottom coordinate of bottom horizontal clipping plane
		*/
		void set_bottom(const GLfloat bottom);

		/*!
		* /param top coordinate of top horizontal clipping plane
		*/
		void set_top(const GLfloat top);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		GLfloat m_left = -10.0f;
		GLfloat m_right = 10.0f;
		GLfloat m_bottom = -10.0f;
		GLfloat m_top = 10.0f;
	};
} 
#endif