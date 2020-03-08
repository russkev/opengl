#include "pch.h"
#include "ShadowMap.h"

#include <glm/gtc/matrix_transform.hpp>

#include "light/Light.h"
#include "light/DirectionalLight.h"
#include "light/PointLight.h"
#include "light/SpotLight.h"
#include "node/LightNode.h"
#include "node/MeshNode.h"
#include "material/MaterialLibrary.h"

namespace glen
{
	// // ----- CONSTANTS ----- // //

	//const GLuint ShadowMap::k_shadow_width = 1024;
	//const GLuint ShadowMap::k_shadow_height = 1024;
	const glm::uvec2 ShadowMap::k_shadow_dimensions = glm::uvec2{ 1024, 1024 };
	const GLfloat ShadowMap::k_default_clip_near = 0.1f;
	const GLfloat ShadowMap::k_default_clip_far = 100.0f;

	// // ----- CONSTRUCTORS ----- // //
	ShadowMap::ShadowMap(LightNode* light_node) :
		m_texture{ Texture::create_depth_null_texture_for_shadow(GL_TEXTURE_2D_ARRAY, k_shadow_dimensions) },
		m_light_node{ light_node },
		m_camera_node{ std::string(light_node->name() + " shadow"), light_node->light()->camera() }
	{
		init_camera();

		light_node->set_shadowMap(this);
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
			std::string index = std::to_string(m_light_node->shader_pos());
			std::string type = m_light_node->light()->type();

			if (BlinnMaterial* blinn_material = dynamic_cast<BlinnMaterial*>(material))
			{
				material->set_texture(type + "[" + index + "]." + BlinnMaterial::k_depth, &m_texture);
				if (is_point())
				{
					material->set_uniform(type + "[" + index + "]." + BlinnMaterial::k_far_plane, m_camera_node.camera()->clip_far());
				}
				material->set_uniform(type + "[" + index + "]." + BlinnMaterial::k_shadow_enabled, true);
			}
		}
	}

	void ShadowMap::init_camera()
	{
		m_camera_node.set_parent(m_light_node);
		m_camera_node.camera()->set_dimensions(k_shadow_dimensions);
		m_camera_node.camera()->set_clip_near(k_default_clip_near);
		m_camera_node.camera()->set_clip_far(k_default_clip_far);
	}

	void ShadowMap::init_directional_shadowMap()
	{
		//glm::uvec2 dimensions{ k_shadow_width, k_shadow_height };
		m_texture = Texture::create_depth_null_texture_for_shadow(GL_TEXTURE_2D_ARRAY, k_shadow_dimensions);

		m_texture.bind();
		m_framebuffer.bind();

		m_framebuffer.process_texture(&m_texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		m_framebuffer.check_bound_framebuffer();

		m_framebuffer.unbind();

		// Create depth material
		m_depth_material = std::make_unique<DepthMaterial>();
		m_texture.unbind();
	}

	void ShadowMap::init_point_shadowMap()
	{
		//glm::uvec2 dimensions{ k_shadow_width, k_shadow_height };
		m_texture = std::move(Texture::create_depth_null_texture_for_shadow(GL_TEXTURE_CUBE_MAP, k_shadow_dimensions));

		m_texture.bind();
		m_framebuffer.bind();

		m_framebuffer.process_texture(&m_texture);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		m_framebuffer.check_bound_framebuffer();
		m_framebuffer.unbind();

		// Create depth material
		m_depth_material = std::make_unique<DepthCubeMaterial>();
		m_texture.unbind();
	}

	// // ----- UPDATE ----- // //
	void ShadowMap::update_materials(std::vector<Material*>& materials)
	{
		for (Material* material : materials)
		{
			std::string index = std::to_string(m_light_node->shader_pos());
			std::string type = m_light_node->light()->type();

			if (is_directional())
			{
				material->update_light_transform(m_light_node, &m_camera_node);
			}
		}
	}

	// // ----- RENDER ----- // //
	void ShadowMap::render_shadowMap(std::map<std::string, Node*>& root_nodes)
	{
		m_texture.bind();
		m_framebuffer.bind();

		glViewport(0, 0, k_shadow_dimensions.x, k_shadow_dimensions.y);
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
		std::string type = m_light_node->light()->type();
		std::string index = std::to_string(m_light_node->shader_pos());


		for (auto const& node_pair : root_nodes)
		{
			Node* node = node_pair.second;
			DepthMaterial* depth_material = dynamic_cast<DepthMaterial*>(m_depth_material.get());
			node->update_view(&m_camera_node);

			if (MeshNode* meshNode = dynamic_cast<MeshNode*>(node))
			{
				depth_material->update_view(&m_camera_node, meshNode);
				meshNode->draw_material(depth_material);
			}
		}
	}

	void ShadowMap::render_point_shadowMap(std::map<std::string, Node*>& root_nodes)
	{
		DepthCubeMaterial* depth_cube_material = dynamic_cast<DepthCubeMaterial*>(m_depth_material.get());
		std::vector<glm::mat4> shadow_transforms = make_poin_shadow_transforms(m_light_node->world_position());


		depth_cube_material->use();

		for (auto const& node_pair : root_nodes)
		{
			Node* node = node_pair.second;
			node->update_view(&m_camera_node);
			if (MeshNode* meshNode = dynamic_cast<MeshNode*>(node))
			{
				depth_cube_material->update_view(&m_camera_node, meshNode);
				for (GLuint i = 0; i < 6; ++i)
				{
					std::string index = std::to_string(i);
					depth_cube_material->set_uniform(DepthCubeMaterial::k_shadow + "[" + index + "]." + DepthCubeMaterial::k_transform, shadow_transforms.at(i));
				}
				meshNode->draw_material(depth_cube_material);
			}
		}
	}

	std::vector<glm::mat4> ShadowMap::make_poin_shadow_transforms(const glm::vec3& position)
	{
		std::vector<glm::mat4> out_transforms;
		out_transforms.push_back(m_camera_node.camera()->cam_to_projection() *
			glm::lookAt(position, position + glm::vec3(+1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, -1.0f, +0.0f)));

		out_transforms.push_back(m_camera_node.camera()->cam_to_projection() *
			glm::lookAt(position, position + glm::vec3(-1.0f, +0.0f, +0.0f), glm::vec3(+0.0f, -1.0f, +0.0f)));

		out_transforms.push_back(m_camera_node.camera()->cam_to_projection() *
			glm::lookAt(position, position + glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, +1.0f)));

		out_transforms.push_back(m_camera_node.camera()->cam_to_projection() *
			glm::lookAt(position, position + glm::vec3(+0.0f, -1.0f, +0.0f), glm::vec3(+0.0f, +0.0f, -1.0f)));

		out_transforms.push_back(m_camera_node.camera()->cam_to_projection() *
			glm::lookAt(position, position + glm::vec3(+0.0f, +0.0f, +1.0f), glm::vec3(+0.0f, -1.0f, +0.0f)));

		out_transforms.push_back(m_camera_node.camera()->cam_to_projection() *
			glm::lookAt(position, position + glm::vec3(+0.0f, +0.0f, -1.0f), glm::vec3(+0.0f, -1.0f, +0.0f)));

		return out_transforms;
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
		return m_light_node->light()->type() == DirectionalLight::TYPE || m_light_node->light()->type() == SpotLight::TYPE;
	}

	bool ShadowMap::is_point()
	{
		return m_light_node->light()->type() == PointLight::TYPE;
	}

	// // ----- SETTERS ----- // //
	void ShadowMap::set_clip_near(GLfloat clip_near)
	{
		m_camera_node.camera()->set_clip_near(clip_near);
	}
	void ShadowMap::set_clip_far(GLfloat clip_far)
	{
		m_camera_node.camera()->set_clip_far(clip_far);
	}
}