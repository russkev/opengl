#include "OrthoCamera.h"

#include "../utils/VectorUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gl_engine
{
	OrthoCamera::OrthoCamera()
	{}

	OrthoCamera::OrthoCamera(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top) :
		m_left(left), m_right(right), m_bottom(bottom), m_top(top)
	{}


	// // ----- CAMERA MOVEMENT ----- // //
	void OrthoCamera::update()
	{

	}

	// // ----- GETTERS ----- // //
	glm::mat4 OrthoCamera::transform_to_cam(const glm::mat4& transform)
	{
		glm::vec3 position	= VectorUtils::extract_position(transform);
		glm::vec3 right		= glm::vec3{ transform[0][0], transform[0][1], transform[0][2] };
		glm::vec3 up		= glm::vec3{ transform[1][0], transform[1][1], transform[1][2] };
		glm::vec3 forward	= glm::vec3{ transform[2][0], transform[2][1], transform[2][2] };

		return glm::lookAt(position, position + forward, up);
	}

	glm::mat4 OrthoCamera::cam_to_projection()
	{
		return glm::ortho(m_left, m_right, m_bottom, m_top, Camera::clip_near(), Camera::clip_far());
	}

	// // ----- SETTERS ----- // //
	void OrthoCamera::set_sides(const GLfloat left, const GLfloat right, const GLfloat bottom, const GLfloat top)
	{
		m_left = left;
		m_right = right;
		m_bottom = bottom;
		m_top = top;
	}

	void OrthoCamera::set_left(const GLfloat left)
	{
		m_left = left;
	}

	void OrthoCamera::set_right(const GLfloat right)
	{
		m_right = right;
	}

	void OrthoCamera::set_bottom(const GLfloat bottom)
	{
		m_bottom = bottom;
	}

	void OrthoCamera::set_top(const GLfloat top)
	{
		m_top = top;
	}
} // namespace gl_engine