#ifndef GL_ENGINE_CAMERA_TARGET_CAMERA_H
#define GL_ENGINE_CAMERA_TARGET_CAMERA_H

#include "Camera.h"

namespace glen
{
	/*

		Target camera. Camera orbits around a particular point.
		Pressing 'F' or 'Z' focuses the camera on the focus target.

	*/
	struct TargetCamera : public Camera
	{

		// // ----- CONSTANTS ----- // //
	private:
		inline static const glm::vec3 UP_AXIS = { 0.0f, 1.0f, 0.0f };
		inline static const glm::vec3 MOVE_SPEED = { 1.0f, 0.005f, 0.005f };
		static constexpr float ROTATION_SPEED = 0.007f;


		// // ----- CONSTRUCTOR ----- // //
	public:
		/*!
		* Constructor
		*/
		TargetCamera();

		// // ----- CAMERA MOVEMENT ----- // //
		/*!
		* Update the camera position.
		* Accept input from the user to move the camera around.
		* Use left, middle and right button to rotate around target, 
		* move camera + target and zoom respectively
		* Pressing `F` or `Z` will cause the camera to 'look at' the focus target.
		*/
		void update() override;

	private:
		void move_relative(const glm::vec3& moveDelta);
		void rotate_relative(const glm::vec2& rotateDelta);
		void print_data(const glm::vec2& rotateDelta);
		void focus(const glm::vec3& focusTarget);


		// // ----- GETTERS ----- // //
	public:

		/*!
		 * @brief Reposition camera with matrix
		 * @param transform The input transformation matrix
		 * @return The new local transformation matrix
		*/
		const glm::mat4 transform_to_cam(const glm::mat4& transform) const override;

		/*!
		 * @return The camera's perspective transfrom matrix
		*/
		const glm::mat4 cam_to_projection() const override;


		// // ----- SETTERS ----- // //
		
		/*!
		 * @brief Set the direction the camera is pointing
		 * @param direction The direction vector
		*/
		void set_view_direction(glm::vec3 direction);

		/*!
		 * @brief Set the right direction of the camera
		 * An alternative to using `set_view_direction`
		 * @param direction The direction vector
		*/
		void set_cam_right(glm::vec3 direction);

		/*!
		 * @brief Set the angle of view in the y (up/down) direction
		 * @param angle The angle in radians
		*/
		void set_angle_of_view(GLfloat angle);

		/*!
		 * @brief The point that the camera is looking at.
		 * When the `F` is pressed, this is where the camera will focus.
		 * @param target The target to 'look at'.
		*/
		void set_focus_target(const glm::vec3& target);

	private:
		void set_look_target();
		void set_cam_up();

		// // ----- MEMBER VARIABLES ----- // //
	private:
		glm::vec3 m_view_direction = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_cam_right = { -1.0f, 0.0f, 0.0f };
		glm::vec3 m_look_target = m_view_direction + m_cam_right;
		glm::vec3 m_focus_target = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_cam_up = glm::cross(m_view_direction, m_cam_right);
		glm::vec2 m_old_mouse_position;
		GLfloat m_angle_of_view = 60.0f; // In degrees
	};
}
#endif