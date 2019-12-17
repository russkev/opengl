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
		glm::vec3 m_position = { 0.0f, 8.0f, 8.0f };
		//glm::vec3 m_viewDirection = { 0.0f, -1.0f, -1.0f };
		glm::vec3 m_viewDirection = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_camRight = { -1.0f, 0.0f, 0.0f };

		glm::vec3 m_lookTarget = m_viewDirection + m_camRight;
		glm::vec3 m_focusTarget = { 0.0f, 0.0f, 0.0f };
		//glm::vec3 m_lookTarget = glm::vec3{ 0.0f, 0.0f, 0.0f };

		glm::vec3 m_camUp = glm::cross(m_viewDirection, m_camRight);

		glm::vec2 m_oldMousePosition;

		GLfloat m_angleOfView = 60.0f; // In degrees

		// // ----- CONSTRUCTOR ----- // //
	public:
		TargetCamera();

		// // ----- CAMERA MOVEMENT ----- // //
		void update(glm::mat4* transform) override;

		//void positionUpdate(const SDL_Scancode& newPosition);
		void scrollUpdate(const float scrollAmount);
		void moveRel(const glm::vec3& moveDelta);
		void rotateRel(const glm::vec2& rotateDelta);
		void printData(const glm::vec2& rotateDelta);
		void focus(const glm::vec3& focusTarget);

	private:
		void updatePosition(glm::vec3 newPosition);

		// // ----- GETTERS ----- // //
		const glm::vec3 position() const override;
		glm::mat4 objectToCam_matrix() override;
		glm::mat4 viewToProjection_matrix() override;
		glm::mat4 worldToProjection_matrix() override;

		// // ----- SETTERS ----- // //
		void setPosition(glm::vec3);
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