#include "GL_Scene.h"



void GL_Scene::init(const GLuint width, const GLuint height)
{
	m_width		= width;
	m_height	= height;

	printGLProperties();
	initVerboseDebug();
	initSettings();
	initCam();
	initLights();
}

void GL_Scene::printGLProperties()
{
	// // Check OpenGL properties // //
	printf("OpenGL loaded\n");
	//gladLoadGLLoader(SDL_GL_GetProcAddress);
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));
}

void GL_Scene::initVerboseDebug()
{
#ifdef DEBUG
	// // Enable the debug callback // //
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	glDebugMessageControl(
		GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE
	);
#endif 
}

void GL_Scene::initSettings()
{
	// // Dark blue background // //
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// // Enable depth test // //
	glEnable(GL_DEPTH_TEST);
	// // Enable backface culling // //
	//glEnable(GL_CULL_FACE);
	// // Set winding direction // // 
	glFrontFace(GL_CCW);
	// // Accept fragment shader if it closer to the camera than the previous one
	//glDepthFunc(GL_LESS);
	glDepthFunc(GL_ALWAYS);
	// // Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GL_Scene::initCam()
{
	GLfloat near	= 0.1f;
	GLfloat far		= 100.0f;
	m_projection	= glm::perspective(glm::radians(50.0f), float(m_width) / float(m_height), near, far);
}

void GL_Scene::initLights()
{
	// Initialize lights
}

