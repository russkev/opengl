#include "ShadowMap.h"

#include "../light/Light.h"
#include "../node/MeshNode.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gl_engine
{
	// // ----- CONSTRUCTOR ----- // //
	ShadowMap::ShadowMap()
	{
		init_shadowMap();
	}


	// // ----- SHADOW MAP ----- // //
	void ShadowMap::init_shadowMap()
	{
		// Initialize ortho cam
		m_orthoCam.setClipNear(0.1f);
		m_orthoCam.setClipFar(100.0f);
		m_orthoCam.setSides(-5.0f, 5.0f, -5.0f, 5.0f);

		m_texture.upload_texture();
		m_texture.bind();
		// Create frame buffer object
		glGenFramebuffers(1, &m_depthMap_FBO);
		auto depthTexture_ID = m_texture.tex_id();

		// Create 2D texture


		//glGenTextures(1, &m_depthMap_ID);

		////m_depthMap_ID = 1;

		//glBindTexture(GL_TEXTURE_2D, m_depthMap_ID);
		//glTexImage2D(
		//	GL_TEXTURE_2D, 
		//	0, 
		//	GL_DEPTH_COMPONENT, 
		//	SHADOW_WIDTH, 
		//	SHADOW_HEIGHT, 
		//	0, 
		//	GL_DEPTH_COMPONENT, 
		//	GL_FLOAT, 
		//	NULL
		//);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//GLfloat border_color[] = { 1.0f, 0.0f, 0.0f, 0.0f };
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

		// Attach texture to framebuffer's depth buffer

		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture_ID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Create depth shader
		m_depthMaterial = Material("Depth shader", "DepthShader.vert", "DepthShader.frag");
		m_texture.unbind();
	}

	void ShadowMap::update_materials(std::vector<Material*> materials)
	{
		for (Material* material : materials)
		{
			//!!! make string a static
			material->setUniform("transform.worldToLightProjection", m_orthoCam_node.worldToProjection_matrix());

			GLuint tex_location = glGetUniformLocation(material->programID(), "shadowMap");


			glUniform1i(tex_location, 1);
			glActiveTexture(GLenum(GL_TEXTURE0 + 1));
			glBindTexture(GL_TEXTURE_2D, m_depthMap_FBO);
		}
	}

	void ShadowMap::render_shadowMap(std::map<std::string, Node*>& root_nodes)
	{
		m_orthoCam_node.update();

		// 1. Render depth map
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		m_depthMaterial.setUniform("lightSpace_matrix", m_orthoCam_node.worldToProjection_matrix());
		for (auto const& node : root_nodes)
		{
			node.second->update_view(&m_orthoCam_node);
			node.second->draw(Node::shadow);
		}
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}



	// // ----- SETTERS ----- // //
	void ShadowMap::setLightNode(Node* light_node)
	{
		m_orthoCam_node.setParent(light_node);
	}



} // namespace gl_engine