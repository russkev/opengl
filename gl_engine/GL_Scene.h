#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <iostream>

#include "Camera/Camera.h"
#include "Light/PointLight.h"
#include "ShapeContainer.h"
#include "utils/VectorUtils.h"
#include "shading/LoadShader.h"
#include "Buffer.h"
#include "VAO.h"
#include "Timer.h"
#include "shading/Bitmap.h"
#include "shading/Texture.h"
#include "shading/Targa.h"
#include "shading/Text2D.h"
#include "shading/Shader.h"
#include "mesh/Mesh.h"
#include "shading/Material.h"
#include "node/MeshNode.h"
#include "node/CameraNode.h"

#include "GL_Type_Traits.h"
#include "GL_Tuple_Introspect.h"


//#define DEBUG
static constexpr auto POSITION_ATTR		= 0u;
static constexpr auto COLOR_ATTR		= 1u;
static constexpr auto NORMAL_ATTR		= 2u;
static constexpr auto UV_ATTR			= 3u;
static constexpr auto CAM_ATTR			= 4u;
static constexpr auto TANGENT_ATTR		= 5u;
static constexpr auto BITANGENT_ATTR	= 6u;
static constexpr auto MODEL_ATTR		= 7u;
static constexpr auto WORLD_ATTR		= 11u;

struct GL_Scene
{
	void init(const GLuint width, const GLuint height);
	void printGLProperties();
	void initVerboseDebug();
	void initSettings();
	void initTimer();
	void initCam();
	void initLights();
	void initText();
	void initGeo();
	void initBuffers();

	bool pollEvents();

	void renderFrame();
	void updateTimer();
	void updateLights();
	void updateCam();
	void updateGeo();

	~GL_Scene();


public:
	GLuint			m_program_id, m_text_program_id, m_light_program_id;
	glm::mat4		m_projection	= glm::mat4();
	//Camera			m_cam;

	PointLight		m_light;
	GLuint			m_width, m_height;
	ShapeContainer	m_sh;// , m_light_sh;
	VAO				m_vao_main, m_vao_text, m_vao_light;
	Timer			m_timer;
	bool			m_displayWireframe = false;

	// BUFFERS
	Buffer m_geoBuffer	= { GL_ARRAY_BUFFER, 0 };
	Buffer m_matBuffer	= { GL_ARRAY_BUFFER, 0 };
	Buffer m_wldBuffer	= { GL_ARRAY_BUFFER, 0 };
	Buffer m_viewBuffer = { GL_ARRAY_BUFFER, 0 };
	Buffer m_indxBuffer	= { GL_ARRAY_BUFFER, 0 };

	Buffer m_textBuffer = { GL_ARRAY_BUFFER, 0 };

	Buffer m_light_indexBuffer	= { GL_ARRAY_BUFFER, 0 };
	Buffer m_light_geoBuffer	= { GL_ARRAY_BUFFER, 0 };
	Buffer m_light_matBuffer	= { GL_ARRAY_BUFFER, 0 };

	Text2D m_text		= { "font_calibri_01.tga" };


	Mesh m_mesh1;
	CameraNode m_camNode;
	Material m_material1;
	MeshNode m_meshNode1;
};