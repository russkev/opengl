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

		// Initialize texture settings
		m_texture.set_width(SHADOW_WIDTH);
		m_texture.set_height(SHADOW_HEIGHT);
		m_texture.set_internalFormat(GL_DEPTH_COMPONENT);
		m_texture.set_format(GL_DEPTH_COMPONENT);
		m_texture.set_type(GL_FLOAT);
		m_texture.set_minFilter(GL_NEAREST);
		m_texture.set_magFilter(GL_LINEAR);
		m_texture.set_mipmap(false);
		m_texture.set_st_wrap(GL_CLAMP_TO_BORDER);

		// Process texture
		m_texture.process();

		// Create frame buffer object
		glGenFramebuffers(1, &m_depthMap_FBO);

		m_texture.bind();
		// Attach texture to framebuffer's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
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
		m_texture.bind();
		m_orthoCam_node.update();

		// 1. Render depth map
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		m_depthMaterial.setUniform("lightSpace_matrix", m_orthoCam_node.worldToProjection_matrix());
		for (auto const& node_pair : root_nodes)
		{
			Node* node = node_pair.second;
			node->update_view(&m_orthoCam_node);
			if (MeshNode* meshNode = dynamic_cast<MeshNode*>(node))
			{
				m_depthMaterial.setUniform("model", meshNode->worldTransform());
				meshNode->draw_material(&m_depthMaterial);
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ShadowMap::init_materials(std::vector<Material*>& materials)
	{
		for (Material* material : materials)
		{
			material->addTexture("shadowMap", &m_texture);
		}
	}

	void ShadowMap::update_materials(std::vector<Material*>& materials)
	{
		for (Material* material : materials)
		{
			//!!! make string a static
			material->setUniform("transform.worldToLightProjection", m_orthoCam_node.worldToProjection_matrix());
		}
	}


	// // ----- SETTERS ----- // //
	void ShadowMap::setLightNode(Node* light_node)
	{
		m_orthoCam_node.setParent(light_node);
	}



} // namespace gl_engine