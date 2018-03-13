#include "Scene.h"



void Scene::initCam(const GLuint& width, const GLuint& height, const GLfloat& near, const GLfloat& far)
{
	m_projection	= glm::perspective(glm::radians(50.0f), float(width) / float(height), near, far);
	m_view			= m_cam.getWorldToViewMatrix();
}