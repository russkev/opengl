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
#include "Utilities.h"
#include "loadShader.hpp"
#include "Buffer.h"
#include "VAO.h"

#include "GL_Type_Traits.h"
#include "GL_Tuple_Introspect.h"


//#define DEBUG
static constexpr auto POSITION_ATTR = 0u;
static constexpr auto COLOR_ATTR = 1u;
static constexpr auto NORMAL_ATTR = 2u;
static constexpr auto CAM_ATTR = 3u;
static constexpr auto MODEL_ATTR = 4u;
static constexpr auto WORLD_ATTR = 8u;

struct GL_Scene
{
	void init(const GLuint width, const GLuint height);
	void printGLProperties();
	void initVerboseDebug();
	void initSettings();
	void initCam();
	void initLights();
	void initGeo();
	void initBuffers();

	void render_frame();
	void prepareLights();
	void prepareCam();
	void prepareGeo();

	~GL_Scene();


public:
	GLuint			m_program_id;
	glm::mat4		m_projection	= glm::mat4();
	Camera			m_cam;
	GLuint			m_width, m_height;
	ShapeGenerator	m_shapes;
	ShapeContainer	m_sh;
	VAO				m_vao_main;// , m_vao_mat;

	// BUFFERS
	Buffer m_geoBuffer	= { GL_ARRAY_BUFFER, 0 };
	Buffer m_matBuffer	= { GL_ARRAY_BUFFER, 0 };
	Buffer m_wldBuffer	= { GL_ARRAY_BUFFER, 0 };
	Buffer m_indxBuffer	= { GL_ARRAY_BUFFER, 0 };
};