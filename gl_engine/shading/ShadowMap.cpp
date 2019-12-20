#include "ShadowMap.h"

#include "../light/Light.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gl_engine
{
	// // ----- CONSTRUCTOR ----- // //
	ShadowMap::ShadowMap()
	{}


	// // ----- SHADOW MAP ----- // //
	void ShadowMap::init_shadowMap()
	{
		// Create frame buffer object
		glGenFramebuffers(1, &m_depthMap_FBO);

		// Create 2D texture
		//const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

		glGenTextures(1, &m_depthMap_ID);
		glBindTexture(GL_TEXTURE_2D, m_depthMap_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Attach texture to framebuffer's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap_ID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Create depth shader
		m_depthShader = Shader("Depth shader", "DepthShader.vert", "DepthShader.frag");
	}

	void ShadowMap::render_shadowMap(const glm::mat4& model_matrix)
	{
		// 1. Render depth map
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Configure shader and matrices
		//GLfloat near_plane = 1.0f, far_plane = 7.5f;
		//glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		//glm::mat4 lightView = glm::lookAt(
		//	glm::vec3(-2.0f, 4.0f, -1.0f),
		//	glm::vec3(0.0f, 0.0f, 0.0f),
		//	glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 lightSpace_matrix = lightProjection * lightView;



		// Render shadow map
		//m_depthShader.setUniform("lightSpace_matrix", lightSpace_matrix);
		//m_depthShader.setUniform("model", model_matrix);

		m_depthShader.setUniform("lightSpace_matrix", m_orthoCam_node.worldToProjection_matrix());
		//m_depthShader.setUniform("model")

		m_depthShader.use();
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
		glClear(GL_DEPTH_BUFFER_BIT);



		// 2. Render
	}

	// // ----- SETTERS ----- // //
	void ShadowMap::setLightNode(Node* light_node)
	{
		m_orthoCam_node.setParent(light_node);
	}



} // namespace gl_engine