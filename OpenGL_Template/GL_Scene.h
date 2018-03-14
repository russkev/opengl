#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <iostream>

#include "Camera.h"
#include "ShapeGenerator.h"
#include "ShapeContainer.h"

//#define DEBUG


struct GL_Scene
{
	void init(const GLuint width, const GLuint height);
	void printGLProperties();
	void initVerboseDebug();
	void initSettings();
	void initCam();
	void initLights();


public:
	glm::mat4		m_projection	= glm::mat4();
	Camera			m_cam;
	GLuint			m_width, m_height;
	ShapeGenerator	m_shapes;
	ShapeContainer	m_sh;

};