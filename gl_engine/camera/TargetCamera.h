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
		static const glm::vec3 UP_AXIS;
		static const float MOVE_SPEED;
		static const float MOUSE_MOVE_SPEED;
		static const float ROTATION_SPEED;

		// // ----- CONSTRUCTOR ----- // //
	public:
		TargetCamera();

		// // ----- CAMERA MOVEMENT ----- // //
		void update() override;
		void move_relative(const glm::vec3& moveDelta);
		void rotate_relative(const glm::vec2& rotateDelta);
		void print_data(const glm::vec2& rotateDelta);
		void focus(const glm::vec3& focusTarget);

		// // ----- GETTERS ----- // //
		const glm::mat4 transform_to_cam(const glm::mat4& transform) const override;
		const glm::mat4 cam_to_projection() const override;


		// // ----- SETTERS ----- // //
		void set_view_direction(glm::vec3);
		void set_cam_right(glm::vec3);
		void set_angle_of_view(GLfloat);
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
} // namespace gl_engine
#endif