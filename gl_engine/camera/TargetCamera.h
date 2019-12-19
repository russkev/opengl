#ifndef GL_ENGINE_CAMERA_TARGET_CAMERA_H
#define GL_ENGINE_CAMERA_TARGET_CAMERA_H

#include "Camera.h"

namespace gl_engine
{
	/*

		Target camera. Camera orbits around a particular point.
		Pressing 'F' or 'Z' focuses the camera on the focus target.

	*/
	struct TargetCamera : public Camera
	{
	private:
		static const glm::vec3 UP_AXIS;
		static const float MOVE_SPEED;
		static const float MOUSE_MOVE_SPEED;
		static const float ROTATION_SPEED;

		// // ----- MEMBER VARIABLES ----- // //
		glm::vec3 m_viewDirection = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_camRight = { -1.0f, 0.0f, 0.0f };
		glm::vec3 m_lookTarget = m_viewDirection + m_camRight;
		glm::vec3 m_focusTarget = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_camUp = glm::cross(m_viewDirection, m_camRight);
		glm::vec2 m_oldMousePosition;
		GLfloat m_angleOfView = 60.0f; // In degrees

		// // ----- CONSTRUCTOR ----- // //
	public:
		TargetCamera();

		// // ----- CAMERA MOVEMENT ----- // //
		void update() override;
		void moveRel(const glm::vec3& moveDelta);
		void rotateRel(const glm::vec2& rotateDelta);
		void printData(const glm::vec2& rotateDelta);
		void focus(const glm::vec3& focusTarget);

		// // ----- GETTERS ----- // //
		glm::mat4 transformToCam_matrix(const glm::mat4& transform) override;
		glm::mat4 camToProjection_matrix() override;


		// // ----- SETTERS ----- // //
		void setViewDirection(glm::vec3);
		void setCamRight(glm::vec3);
		void setAngleOfView(GLfloat);
		void setFocusTarget(const glm::vec3& target);

	private:
		void setLookTarget();
		void setCamUp();
	};
} // namespace gl_engine
#endif