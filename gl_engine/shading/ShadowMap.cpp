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

		// // ----- NEW WAY ----- // //

		//m_texture.set_minFilter(GL_NEAREST);
		//m_texture.set_magFilter(GL_LINEAR);
		//m_texture.process();
		//// Create frame buffer object
		//glGenFramebuffers(1, &m_depthMap_FBO);
		//auto depthTexture_ID = 1;// m_texture.id();
		//m_texture.bind(1);

		//glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture_ID, 0);
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//// Create depth shader
		//m_depthMaterial = Material("Depth shader", "DepthShader.vert", "DepthShader.frag");
		//m_texture.unbind();

		// // ----- NEW WAY ----- // //
		//m_texture = Texture{ SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, NULL };
		m_texture.set_minFilter(GL_NEAREST);
		m_texture.set_magFilter(GL_LINEAR);
		m_texture.set_mipmap(false);
		m_texture.process();

		GLuint temp_id = m_texture.id();


		// // -----OLD WAY ----- // //

		// Create frame buffer object
		glGenFramebuffers(1, &m_depthMap_FBO);

		// Create 2D texture
		//m_depthMap_FBO = 1;

		//glGenTextures(1, &m_depthMap_ID);

		//m_depthMap_ID = m_texture.id();

		//glBindTexture(GL_TEXTURE_2D, m_depthMap_ID);
		//m_texture.bind();
		//m_texture.process();
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

		m_texture.bind();
		// Attach texture to framebuffer's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, temp_id, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture.id(), 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Create depth shader
		m_depthMaterial = Material("Depth shader", "DepthShader.vert", "DepthShader.frag");
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void ShadowMap::render_shadowMap(std::map<std::string, Node*>& root_nodes)
	{
		//glActiveTexture(GLenum(GL_TEXTURE0 + m_depthMap_ID));
		//glBindTexture(GL_TEXTURE_2D, m_depthMap_ID);

		m_texture.bind();

		// // ----- OLD WAY ----- // //
		m_orthoCam_node.update();

		// 1. Render depth map
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		m_depthMaterial.setUniform("lightSpace_matrix", m_orthoCam_node.worldToProjection_matrix());
		for (auto const& node : root_nodes)
		{
			node.second->update_view(&m_orthoCam_node);
			if (MeshNode* meshNode = dynamic_cast<MeshNode*>(node.second))
			{
				//m_depthMaterial.setUniform("lightSpace_matrix", meshNode->worldTransform());
				//meshNode->draw();
				meshNode->draw_material(&m_depthMaterial);
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		// // ----- NEW WAY ----- // //

		//m_orthoCam_node.update();
		//m_depthMaterial.setUniform("lightSpace_matrix", m_orthoCam_node.worldToProjection_matrix());


		//glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		//glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
		//glClear(GL_DEPTH_BUFFER_BIT);


		//for (auto const& node_pair : root_nodes)
		//{
		//	Node* node = node_pair.second;
		//	node->update_view(&m_orthoCam_node);
		//	

		//	//DRAW
		//	if (MeshNode* mesh_node = dynamic_cast<MeshNode*>(node))
		//	{
		//		m_depthMaterial.setUniform("lightSpace_matrix", mesh_node->worldTransform());
		//		//mesh_node->draw();
		//		mesh_node->draw_material(&m_depthMaterial);
		//	}
		//}
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);




	}

	void ShadowMap::update_materials(std::vector<Material*> materials)
	{
		// // ----- NEW WAY ----- // //

		for (Material* material : materials)
		{
			//GLuint temp_id = 1;
			//!!! make string a static
			material->setUniform("transform.worldToLightProjection", m_orthoCam_node.worldToProjection_matrix());

			GLuint tex_location = glGetUniformLocation(material->programID(), "shadowMap");


			//glUniform1i(tex_location, temp_id);
			//glActiveTexture(GLenum(GL_TEXTURE0 + temp_id));
			//m_texture.bind();
			material->addTexture("shadowMap", &m_texture);
			//glBindTexture(GL_TEXTURE_2D, m_depthMap_FBO);
		}


		// // -----OLD WAY ----- // //

	}


	// // ----- SETTERS ----- // //
	void ShadowMap::setLightNode(Node* light_node)
	{
		m_orthoCam_node.setParent(light_node);
	}



} // namespace gl_engine