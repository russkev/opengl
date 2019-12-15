#include "Camera.h"

namespace gl_engine
{
	// // ----- GETTERS ----- // //

	const glm::uvec2& Camera::dimensions() const
	{
		return m_dimensions;
	}

	const GLfloat& Camera::clipNear() const
	{
		return m_clipNear;
	}
	const GLfloat& Camera::clipFar() const
	{
		return m_clipFar;
	}

	const glm::vec3 Camera::position() const
	{
		return glm::vec3(0.0f);
	}

	glm::mat4 Camera::worldToProjection_matrix()
	{
		return viewToProjectionMatrix() * worldToCam_matrix();
	}


	// // ----- SETTERS ----- // //
	void Camera::setDimensions(glm::uvec2 dimensions)
	{
		m_dimensions = dimensions;
	}

	void Camera::setClipNear(GLfloat clipNear)
	{
		m_clipNear = clipNear;
	}

	void Camera::setClipFar(GLfloat clipFar)
	{
		m_clipFar = clipFar;
	}
} // namespace gl_engine