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

	const std::string ShadowMap::DEPTH_MAP_NAME = "depth map";
	const char* ShadowMap::DEPTH_MAP_VERT = "DepthShader.vert";
	const char* ShadowMap::DEPTH_MAP_FRAG = "DepthShader.frag";
	const char* ShadowMap::CUBE_MAP_VERT = "DepthShaderCube.vert";
	const char* ShadowMap::CUBE_MAP_GEOM = "DepthShaderCube.geom";
	const char* ShadowMap::CUBE_MAP_FRAG = "DepthShaderCube.frag";

	const std::string ShadowMap::SHADOW = "shadow";
	const std::string ShadowMap::TRANSFORMS = "transforms";
	const std::string ShadowMap::FAR_PLANE = "far_plane";

	// // ----- CONSTRUCTOR ----- // //
	ShadowMap::ShadowMap(LightNode* lightNode) :
		m_camera_node{ std::string(lightNode->name() + " shadow"), lightNode->light()->camera() },
		m_lightNode{ lightNode }
	{
		m_camera_node.setParent(lightNode);
		lightNode->set_shadowMap(this);
		if (is_directional())
		{
			init_directional_shadowMap();
		}
		else if (is_point())
		{
			init_point_shadowMap();
		}
	}


	// // ----- SHADOW MAP ----- // //
	void ShadowMap::init_directional_shadowMap()
	{
		m_texture = Texture{ GL_TEXTURE_2D_ARRAY };

		// Initialize cam
		m_camera_node.camera()->setClipNear(0.1f);
		m_camera_node.camera()->setClipFar(100.0f);

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
		m_depthMaterial = Material(DEPTH_MAP_NAME, DEPTH_MAP_VERT, DEPTH_MAP_FRAG);
		m_texture.unbind();
	}

	void ShadowMap::init_point_shadowMap()
	{
		//m_texture = Texture{ GL_TEXTURE_2D_ARRAY };
		m_texture = Texture{ GL_TEXTURE_CUBE_MAP };

		// Initialize cam
		m_camera_node.camera()->setClipNear(1.0f);
		m_camera_node.camera()->setClipFar(25.0f);

		//// Initialize texture settings
		//m_texture.set_width(SHADOW_WIDTH);
		//m_texture.set_height(SHADOW_HEIGHT);
		//m_texture.set_internalFormat(GL_DEPTH_COMPONENT);
		//m_texture.set_format(GL_DEPTH_COMPONENT);
		//m_texture.set_type(GL_FLOAT);
		//m_texture.set_minFilter(GL_NEAREST);
		//m_texture.set_magFilter(GL_NEAREST);
		//m_texture.set_mipmap(false);
		//m_texture.set_st_wrap(GL_CLAMP_TO_EDGE);

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

		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
		//glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texture.id(), 0, 0);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texture.id(), 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		check_bound_framebuffer();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//-------------------------------------------
		m_depthMaterial = Material(DEPTH_MAP_NAME, CUBE_MAP_VERT, CUBE_MAP_GEOM, CUBE_MAP_FRAG);
		m_texture.unbind();
	}

	void ShadowMap::render_shadowMap(std::map<std::string, Node*>& root_nodes)
	{
		m_texture.bind();

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMap_FBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		if (is_directional())
		{
			render_directional_shadowMap(root_nodes);
		}
		else if (is_point())
		{
			render_point_shadowMap(root_nodes);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_texture.unbind();
	}

	void ShadowMap::render_directional_shadowMap(std::map<std::string, Node*>& root_nodes)
	{
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
	}

	void ShadowMap::render_point_shadowMap(std::map<std::string, Node*>& root_nodes)
	{
		// !!! Use a camera node for this
		//GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
		//GLfloat near = 1.0f;
		//GLfloat far = 25.0f; 
		//glm::mat4 shadow_projection = glm::perspective(glm::radians(90.0f), aspect, near, far);

		m_depthMaterial.use();

		std::vector<glm::mat4> shadow_transforms;
		glm::vec3 position = m_lightNode->worldPosition();

		shadow_transforms.push_back(m_camera_node.camera()->camToProjection_matrix() * 
			glm::lookAt(position, position + glm::vec3(+1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, -1.0f, +0.0f)));

		shadow_transforms.push_back(m_camera_node.camera()->camToProjection_matrix() * 
			glm::lookAt(position, position + glm::vec3(-1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, -1.0f, +0.0f)));

		shadow_transforms.push_back(m_camera_node.camera()->camToProjection_matrix() * 
			glm::lookAt(position, position + glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, +1.0f)));

		shadow_transforms.push_back(m_camera_node.camera()->camToProjection_matrix() * 
			glm::lookAt(position, position + glm::vec3(+0.0f, -1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, -1.0f)));

		shadow_transforms.push_back(m_camera_node.camera()->camToProjection_matrix() * 
			glm::lookAt(position, position + glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, -1.0f, +0.0f)));

		shadow_transforms.push_back(m_camera_node.camera()->camToProjection_matrix() * 
			glm::lookAt(position, position + glm::vec3(+0.0f, +0.0f, -1.0f), glm::vec3(+0.0f, -1.0f, +0.0f)));

		for (auto const& node_pair : root_nodes)
		{
			Node* node = node_pair.second;
			node->update_view(&m_camera_node);
			if (MeshNode* meshNode = dynamic_cast<MeshNode*>(node))
			{
				std::string type = m_lightNode->light()->type();
				std::string index = std::to_string(m_lightNode->shaderIndex());

				m_depthMaterial.setUniform(MODEL_TRANSFORM, meshNode->worldTransform());
				for (GLuint i = 0; i < 6; ++i)
				{
					m_depthMaterial.setUniform(SHADOW + "[" + std::to_string(i) + "]." + TRANSFORMS, shadow_transforms.at(i));
				}
				m_depthMaterial.setUniform(type + "." + LightNode::LIGHT_POSITION, m_lightNode->worldPosition());
				m_depthMaterial.setUniform(type + "." + FAR_PLANE, m_camera_node.camera()->clipFar());
				meshNode->draw_material(&m_depthMaterial);
			}
		}
	}

	void ShadowMap::init_materials(std::vector<Material*>& materials)
	{
		for (Material* material : materials)
		{
			std::string index = std::to_string(m_lightNode->shaderIndex());
			std::string type = m_lightNode->light()->type();

			material->addTexture(type + "[" + index + "]." + DEPTH_MAP, &m_texture);
			if (is_point())
			{
				material->setUniform(type + "[" + index + "]." + FAR_PLANE, m_camera_node.camera()->clipFar());
			}
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
				printf("FRAMEBUFFER_UNDEFINED \nDefault framebuffer bound but does not exist\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				printf("FRAMEBUFFER_INCOMPLETE_ATTACHMENT \nOne of the framebuffer attachment points is incomplete\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				printf("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT \nFramebuffer does not have at least one image attached\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				printf("FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER \nFramebuffer attachment object type is 'GL_NONE' for a color attachment point named by GL_DRAW_BUFFERi\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				printf("FRAMEBUFFER_INCOMPLETE_READ_BUFFER \nGL_READ_BUFFER is assigned and framebuffer attachment object is GL_NONE for color attachment point named by GL_READ_BUFFER\n");
				return false;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				printf("GL_FRAMEBUFFER_UNSUPPORTED \n A combination of internal formats of attached images violates implementation-dependent restrictions\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				printf("FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				printf("FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS \n A returned framebuffer is layered and a populated attachment is not layered\n");
				return false;
			}
		}
		return true;
	}

	bool ShadowMap::is_directional()
	{
		return m_lightNode->light()->type() == DirectionalLight::TYPE || m_lightNode->light()->type() == SpotLight::TYPE;
	}

	bool ShadowMap::is_point()
	{
		return m_lightNode->light()->type() == PointLight::TYPE;
	}

	// // ----- SETTERS ----- // //
	//void ShadowMap::setLightNode(Node* light_node)
	//{
	//	m_orthoCam_node.setParent(light_node);
	//}



} // namespace gl_engine