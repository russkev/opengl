#include "Camera.h"

#include "../utils/VectorUtils.h"

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
		return VectorUtils::extract_position(*m_transform);
	}

	glm::mat4* Camera::transform()
	{
		return m_transform;
	}

	glm::mat4 Camera::objectToCam_matrix()
	{
		return transformToCam_matrix(*m_transform);
	}

	glm::mat4 Camera::objectToProjection_matrix()
	{
		return transformToProjection_matrix(*m_transform);
	}

	glm::mat4 Camera::transformToProjection_matrix(const glm::mat4& transform)
	{
		return camToProjection_matrix() * transformToCam_matrix(transform);
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

	void Camera::setPosition(const glm::vec3& position)
	{
		VectorUtils::set_position(*m_transform, position);
	}

	void Camera::addPosition(const glm::vec3& position)
	{
		VectorUtils::add_position(*m_transform, position);
	}

	void Camera::registerTransform(glm::mat4* transform)
	{
		m_transform = transform;
	}
} // namespace gl_engine