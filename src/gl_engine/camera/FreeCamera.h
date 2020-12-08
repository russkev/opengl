#ifndef GL_ENGINE_CAMERA_FREE_CAMERA_H
#define GL_ENGINE_CAMERA_FREE_CAMERA_H

#include "Camera.h"

namespace glen
{
	/*!

		Free camera. Not targeted. Orientation is based on parents transforms only

	*/
	struct FreeCamera : public Camera
	{
		// // ----- CONSTANTS ----- // //
	private:
		static const float MOVE_SPEED;

		// // ----- CONSTRUCTOR ----- // //
	public:
		/*
		* Constructor
		*/
		FreeCamera();

		// // ----- CAMERA MOVEMENT ----- // //
		/*!
		* Update the camera position.
		* Accept input from the user to move the camera around.
		* Use `W`, `A`, `S`, `D`, to move forward, backwars, left, right respectively.
		* Use `Q` and `E` to move up and down.
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
		* Set angle of view in the y (up/down) direction
		*/
		void set_angle_of_view(GLfloat angle);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		GLfloat m_angle_of_view = 60.0f;
	};


} 
#endif