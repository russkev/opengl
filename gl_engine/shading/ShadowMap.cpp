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
		m_orthoCam.setClipFar(1000.0f);

		// Create frame buffer object
		glGenFramebuffers(1, &m_depthMap_FBO);

		// Create 2D texture


		glGenTextures(1, &m_depthMap_ID);
		glBindTexture(GL_TEXTURE_2D, m_depthMap_ID);
		glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			GL_DEPTH_COMPONENT, 
			SHADOW_WIDTH, 
			SHADOW_HEIGHT, 
			0, 
			GL_DEPTH_COMPONENT, 
			GL_FLOAT, 
			NULL
		);

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
		m_depthMaterial = Material("Depth shader", "DepthShader.vert", "DepthShader.frag");
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void ShadowMap::update_materials(std::vector<Material*> materials)
	{
		for (Material* material : materials)
		{
			//!!! make string a static
			material->setUniform("transform.worldToLightProjection", m_orthoCam_node.worldToProjection_matrix());
			material->use();
			material->setUniform("material.shadowMap", 1);
			glActiveTexture(GLenum(GL_TEXTURE0 + m_depthMap_ID));
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
			if (MeshNode* meshNode = dynamic_cast<MeshNode*>(node.second))
			{
				m_depthMaterial.setUniform("lightSpace_matrix", meshNode->worldTransform());
				meshNode->draw();
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. Render
	}

	// // ----- SETTERS ----- // //
	void ShadowMap::setLightNode(Node* light_node)
	{
		m_orthoCam_node.setParent(light_node);
	}



} // namespace gl_engine