#include "ShadowMap.h"

#include "../light/Light.h"
#include "../light/DirectionalLight.h"
#include "../light/PointLight.h"
#include "../light/SpotLight.h"
#include "../node/LightNode.h"
#include "../node/MeshNode.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gl_engine
{
	const std::string ShadowMap::MODEL_TRANSFORM = "transform.modelToWorld";
	const std::string ShadowMap::LIGHT_SPACE_TRANSFORM = "projection";
	const std::string ShadowMap::DEPTH_MAP = "depth";

	// // ----- CONSTRUCTOR ----- // //
	ShadowMap::ShadowMap(LightNode* lightNode) :
		m_camera_node{ std::string(lightNode->name() + " shadow"), lightNode->light()->camera() },
		m_lightNode{ lightNode }
	{
		m_camera_node.setParent(lightNode);
		lightNode->set_shadowMap(this);
		if (lightNode->light()->type() == DirectionalLight::TYPE || lightNode->light()->type() == SpotLight::TYPE)
		{
			init_directional_shadowMap();
		}
		else if (lightNode->light()->type() == PointLight::TYPE)
		{
			init_point_shadowMap();
		}
	}


	// // ----- SHADOW MAP ----- // //
	void ShadowMap::init_directional_shadowMap()
	{
		// Initialize ortho cam

		m_camera_node.camera()->setClipNear(0.1f);
		m_camera_node.camera()->setClipFar(100.0f);
		//m_orthoCam.setSides(-5.0f, 5.0f, -5.0f, 5.0f);

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
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texture.id(), 0, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		check_bound_framebuffer();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Create depth shader
		m_depthMaterial = Material("Depth shader", "DepthShader.vert", "DepthShader.frag");
		m_texture.unbind();
	}

	void ShadowMap::init_point_shadowMap()
	{
		//
	}

	void ShadowMap::render_shadowMap(std::map<std::string, Node*>& root_nodes)
	{
		m_texture.bind();

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		std::string type = m_lightNode->light()->type();
		std::string index = std::to_string(m_lightNode->shaderIndex());

		m_depthMaterial.setUniform(LIGHT_SPACE_TRANSFORM, m_camera_node.worldToProjection_matrix());
		for (auto const& node_pair : root_nodes)
		{
			Node* node = node_pair.second;
			node->update_view(&m_camera_node);
			if (MeshNode* meshNode = dynamic_cast<MeshNode*>(node))
			{
				m_depthMaterial.setUniform(MODEL_TRANSFORM, meshNode->worldTransform());
				meshNode->draw_material(&m_depthMaterial);
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_texture.unbind();
	}

	void ShadowMap::init_materials(std::vector<Material*>& materials)
	{
		for (Material* material : materials)
		{
			std::string index = std::to_string(m_lightNode->shaderIndex());
			std::string type = m_lightNode->light()->type();

			material->addTexture(type + "[" + index + "]." + DEPTH_MAP, &m_texture);
		}
	}

	void ShadowMap::update_materials(std::vector<Material*>& materials)
	{
		for (Material* material : materials)
		{
			std::string index = std::to_string(m_lightNode->shaderIndex());
			std::string type = m_lightNode->light()->type();

			material->setUniform(type + "[" + index + "]." + LIGHT_SPACE_TRANSFORM, m_camera_node.worldToProjection_matrix());
		}
	}

	bool ShadowMap::check_bound_framebuffer()
	{
		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("WARNING: ");
			switch (status)
			{
			case GL_FRAMEBUFFER_UNDEFINED:
				printf("FRAMEBUFFER_UNDEFINED \nDefault framebuffer bound but does not exist");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				printf("FRAMEBUFFER_INCOMPLETE_ATTACHMENT \nOne of the framebiffer attachment points is incomplete");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				printf("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT \nFramebuffer does not have at least one image attached");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				printf("FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER \nFramebuffer attachment object type is 'GL_NONE' for a color attachment point named by GL_DRAW_BUFFERi");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				printf("FRAMEBUFFER_INCOMPLETE_READ_BUFFER \nGL_READ_BUFFER is assigned and framebuffer attachment object is GL_NONE for color attachment point named by GL_READ_BUFFER");
				return false;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				printf("GL_FRAMEBUFFER_UNSUPPORTED \n A combination of internal formats of attached images violates implementation-dependent restrictions");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				printf("FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				printf("FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS \n A returned framebuffer is layered and a populated attachment is not layered");
				return false;
			}
			printf("\n");
		}
		return true;
	}

	// // ----- SETTERS ----- // //
	//void ShadowMap::setLightNode(Node* light_node)
	//{
	//	m_orthoCam_node.setParent(light_node);
	//}



} // namespace gl_engine