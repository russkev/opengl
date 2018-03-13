#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include "Camera.h"


struct Scene
{
	void initCam(const GLuint& width, const GLuint& height, const GLfloat& near = 0.1f, const GLfloat& far = 100.0f);

public:
	glm::mat4	m_projection	= glm::mat4();
	glm::mat4	m_view			= glm::mat4();
	Camera		m_cam;
};