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
	// // ----- CONSTANTS ----- // //

	const GLuint ShadowMap::SHADOW_WIDTH = 1024;
	const GLuint ShadowMap::SHADOW_HEIGHT = 1024;
	const GLfloat ShadowMap::DEFAULT_CLIP_NEAR = 0.1f;
	const GLfloat ShadowMap::DEFAULT_CLIP_FAR = 100.0f;
	const std::string ShadowMap::MODEL_TRANSFORM = "transform.model_to_world";
	const std::string ShadowMap::LIGHT_SPACE_TRANSFORM = "projection";
	const std::string ShadowMap::DEPTH_MAP = "depth";

	const std::string ShadowMap::DEPTH_MAP_NAME = "depth map";
	const char* ShadowMap::DEPTH_MAP_VERT = "Depth.vert";
	const char* ShadowMap::DEPTH_MAP_FRAG = "Depth.frag";
	const char* ShadowMap::CUBE_MAP_VERT = "DepthCube.vert";
	const char* ShadowMap::CUBE_MAP_GEOM = "DepthCube.geom";
	const char* ShadowMap::CUBE_MAP_FRAG = "DepthCube.frag";

	const std::string ShadowMap::SHADOW = "shadow";
	const std::string ShadowMap::TRANSFORMS = "transforms";
	const std::string ShadowMap::FAR_PLANE = "far_plane";

	// // ----- CONSTRUCTORS ----- // //
	ShadowMap::ShadowMap(LightNode* lightNode) :
		m_cameraNode{ std::string(lightNode->name() + " shadow"), lightNode->light()->camera() },
		m_lightNode{ lightNode }
	{
		init_camera();

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


	// // ----- INIT ----- // //
	void ShadowMap::init_materials(std::vector<Material*>& materials)
	{
		for (Material* material : materials)
		{
			std::string index = std::to_string(m_lightNode->shader_pos());
			std::string type = m_lightNode->light()->type();

			material->set_texture(type + "[" + index + "]." + DEPTH_MAP, &m_texture);
			if (is_point())
			{
				material->set_uniform(type + "[" + index + "]." + FAR_PLANE, m_cameraNode.camera()->clip_far());
			}
		}
	}

	void ShadowMap::init_camera()
	{
		m_cameraNode.set_parent(m_lightNode);
		m_cameraNode.camera()->set_dimensions(glm::vec2(SHADOW_WIDTH, SHADOW_HEIGHT));
		m_cameraNode.camera()->set_clip_near(DEFAULT_CLIP_NEAR);
		m_cameraNode.camera()->set_clip_far(DEFAULT_CLIP_FAR);
	}

	void ShadowMap::init_directional_shadowMap()
	{
		m_texture = Texture{ GL_TEXTURE_2D_ARRAY };

		init_texture();

		m_texture.bind();
		m_framebuffer.bind();

		m_framebuffer.process_texture(&m_texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		m_framebuffer.check_bound_framebuffer();

		m_framebuffer.unbind();

		// Create depth shader
		m_depth_material = Material(DEPTH_MAP_NAME, DEPTH_MAP_VERT, DEPTH_MAP_FRAG);
		m_texture.unbind();
	}

	void ShadowMap::init_point_shadowMap()
	{
		//m_texture = Texture{ GL_TEXTURE_2D_ARRAY };
		m_texture = Texture{ GL_TEXTURE_CUBE_MAP };

		init_texture();

		m_texture.bind();
		m_framebuffer.bind();

		m_framebuffer.process_texture(&m_texture);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		m_framebuffer.check_bound_framebuffer();
		m_framebuffer.unbind();

		// Create depth shader
		m_depth_material = Material(DEPTH_MAP_NAME, CUBE_MAP_VERT, CUBE_MAP_GEOM, CUBE_MAP_FRAG);
		m_texture.unbind();
	}

	void ShadowMap::init_texture()
	{
		m_texture.set_width(SHADOW_WIDTH);
		m_texture.set_height(SHADOW_HEIGHT);
		m_texture.set_internal_format(GL_DEPTH_COMPONENT);
		m_texture.set_format(GL_DEPTH_COMPONENT);
		m_texture.set_type(GL_FLOAT);
		m_texture.set_min_filter(GL_NEAREST);
		m_texture.set_mag_filter(GL_NEAREST);
		m_texture.set_mipmap(true);
		m_texture.set_st_wrap(GL_CLAMP_TO_BORDER);

		m_texture.process();
	}

	// // ----- UPDATE ----- // //
	void ShadowMap::update_materials(std::vector<Material*>& materials)
	{
		for (Material* material : materials)
		{
			std::string index = std::to_string(m_lightNode->shader_pos());
			std::string type = m_lightNode->light()->type();

			if (is_directional())
			{
				material->set_uniform(type + "[" + index + "]." + LIGHT_SPACE_TRANSFORM, m_cameraNode.world_to_projection());
			}
		}
	}

	// // ----- RENDER ----- // //
	void ShadowMap::render_shadowMap(std::map<std::string, Node*>& root_nodes)
	{
		m_texture.bind();
		m_framebuffer.bind();

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);

		if (is_directional())
		{
			render_directional_shadowMap(root_nodes);
		}
		else if (is_point())
		{
			render_point_shadowMap(root_nodes);
		}
		m_framebuffer.unbind();
		m_texture.unbind();
	}

	void ShadowMap::render_directional_shadowMap(std::map<std::string, Node*>& root_nodes)
	{
		std::string type = m_lightNode->light()->type();
		std::string index = std::to_string(m_lightNode->shader_pos());

		m_depth_material.set_uniform(LIGHT_SPACE_TRANSFORM, m_cameraNode.world_to_projection());
		for (auto const& node_pair : root_nodes)
		{
			Node* node = node_pair.second;
			node->update_view(&m_cameraNode);
			if (MeshNode* meshNode = dynamic_cast<MeshNode*>(node))
			{
				m_depth_material.set_uniform(MODEL_TRANSFORM, meshNode->world_to_node());
				meshNode->draw_material(&m_depth_material);
			}
		}
	}

	void ShadowMap::render_point_shadowMap(std::map<std::string, Node*>& root_nodes)
	{
		m_depth_material.use();

		std::vector<glm::mat4> shadow_transforms;
		glm::vec3 position = m_lightNode->world_position();

		shadow_transforms.push_back(m_cameraNode.camera()->cam_to_projection() * 
			glm::lookAt(position, position + glm::vec3(+1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, -1.0f, +0.0f)));

		shadow_transforms.push_back(m_cameraNode.camera()->cam_to_projection() * 
			glm::lookAt(position, position + glm::vec3(-1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, -1.0f, +0.0f)));

		shadow_transforms.push_back(m_cameraNode.camera()->cam_to_projection() * 
			glm::lookAt(position, position + glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, +1.0f)));

		shadow_transforms.push_back(m_cameraNode.camera()->cam_to_projection() * 
			glm::lookAt(position, position + glm::vec3(+0.0f, -1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, -1.0f)));

		shadow_transforms.push_back(m_cameraNode.camera()->cam_to_projection() * 
			glm::lookAt(position, position + glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, -1.0f, +0.0f)));

		shadow_transforms.push_back(m_cameraNode.camera()->cam_to_projection() * 
			glm::lookAt(position, position + glm::vec3(+0.0f, +0.0f, -1.0f), glm::vec3(+0.0f, -1.0f, +0.0f)));

		for (auto const& node_pair : root_nodes)
		{
			Node* node = node_pair.second;
			node->update_view(&m_cameraNode);
			if (MeshNode* meshNode = dynamic_cast<MeshNode*>(node))
			{
				std::string type = m_lightNode->light()->type();
				std::string index = std::to_string(m_lightNode->shader_pos());

				m_depth_material.set_uniform(MODEL_TRANSFORM, meshNode->world_to_node());
				for (GLuint i = 0; i < 6; ++i)
				{
					m_depth_material.set_uniform(SHADOW + "[" + std::to_string(i) + "]." + TRANSFORMS, shadow_transforms.at(i));
				}
				m_depth_material.set_uniform(type + "." + LightNode::LIGHT_POSITION, m_lightNode->world_position());
				m_depth_material.set_uniform(type + "." + FAR_PLANE, m_cameraNode.camera()->clip_far());
				meshNode->draw_material(&m_depth_material);
			}
		}
	}

	// // ----- GENERAL METHODS ----- // //
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
	void ShadowMap::set_clip_near(GLfloat clip_near)
	{
		m_cameraNode.camera()->set_clip_near(clip_near);
	}
	void ShadowMap::set_clip_far(GLfloat clip_far)
	{
		m_cameraNode.camera()->set_clip_far(clip_far);
	}
} // namespace gl_engine