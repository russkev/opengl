#include "OrthoCamera.h"

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
	void OrthoCamera::update(glm::mat4* transform)
	{

	}

	// // ----- GETTERS ----- // //
	glm::mat4 OrthoCamera::objectToCam_matrix()
	{
		return glm::mat4(glm::vec4{ 1.0f, 0.0f, 0.0f, 0.0f }, glm::vec4{ 0.0f, 1.0f, 0.0f, 0.0f }, glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f }, glm::vec4{ position(), 1.0f });
	}

	glm::mat4 OrthoCamera::viewToProjection_matrix()
	{
		return glm::ortho(m_left, m_right, m_bottom, m_top, Camera::clipNear(), Camera::clipFar());
	}

	glm::mat4 OrthoCamera::worldToProjection_matrix()
	{
		return viewToProjection_matrix() * objectToCam_matrix();
	}

	// // ----- SETTERS ----- // //
	void OrthoCamera::setSides(const GLfloat left, const GLfloat right, const GLfloat bottom, const GLfloat top)
	{
		m_left = left;
		m_right = right;
		m_bottom = bottom;
		m_top = top;
	}

	void OrthoCamera::setLeft(const GLfloat left)
	{
		m_left = left;
	}

	void OrthoCamera::setRight(const GLfloat right)
	{
		m_right = right;
	}

	void OrthoCamera::setBottom(const GLfloat bottom)
	{
		m_bottom = bottom;
	}

	void OrthoCamera::setTop(const GLfloat top)
	{
		m_top = top;
	}
} // namespace gl_engine