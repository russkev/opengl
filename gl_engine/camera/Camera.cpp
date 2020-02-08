#include "pch.h"
#include "Camera.h"

#include "utils/VectorUtils.h"

namespace glen
{
	// // ----- GETTERS ----- // //

	const glm::uvec2& Camera::dimensions() const
	{
		return m_dimensions;
	}

	const GLfloat& Camera::clip_near() const
	{
		return m_clip_near;
	}
	const GLfloat& Camera::clip_far() const
	{
		return m_clip_far;
	}

	const glm::vec3 Camera::position() const
	{
		return VectorUtils::extract_position(*m_parent_transform);
	}

	const glm::mat4 Camera::transform() const
	{
		return *m_parent_transform;
	}

	const glm::mat4 Camera::object_to_cam() const
	{
		return transform_to_cam(*m_parent_transform);
	}

	const glm::mat4 Camera::object_to_projection() const
	{
		return transform_to_projection(*m_parent_transform);
	}

	const glm::mat4 Camera::transform_to_projection(const glm::mat4& transform) const
	{
		return cam_to_projection() * transform_to_cam(transform);
	}


	// // ----- SETTERS ----- // //
	void Camera::set_dimensions(glm::uvec2 dimensions)
	{
		m_dimensions = dimensions;
	}

	void Camera::set_clip_near(GLfloat clipNear)
	{
		m_clip_near = clipNear;
	}

	void Camera::set_clip_far(GLfloat clipFar)
	{
		m_clip_far = clipFar;
	}

	void Camera::set_position(const glm::vec3& position)
	{
		VectorUtils::set_position(*m_parent_transform, position);
	}

	void Camera::add_to_position(const glm::vec3& position)
	{
		VectorUtils::add_position_to_transform(*m_parent_transform, position);
	}

	void Camera::register_transform(glm::mat4* transform)
	{
		m_parent_transform = transform;
	}
} 