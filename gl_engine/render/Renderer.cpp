#include <stdio.h>

#include "Renderer.h"

#include "../utils/Timer.h"
#include "../Window.h"
#include "../node/Node.h"
#include "../node/MeshNode.h"
#include "../node/CameraNode.h"
#include "../node/LightNode.h"
#include "../camera/Camera.h"
#include "../light/PointLight.h"
#include "../light/SpotLight.h"
#include "../light/DirectionalLight.h"
#include "../shading/Material.h"
#include "../shading/ShadowMap.h"



namespace gl_engine
{
	// // ----- CONSTRUCTORS ----- // //
	Renderer::Renderer(CameraNode* camera, const glm::uvec2& dimensions) :
		m_cameraNode{ camera }, 
		m_dimensions{ dimensions }
	{
		m_cameraNode->camera()->set_dimensions(dimensions);
		init_settings();
		init_hdr();
	}

	// // ----- RENDER ----- // //
	//Draw all nodes to screen
	void Renderer::render()
	{
		if (m_first_frame)
		{
			init_first_frame();
			m_first_frame = false;
		}

		glEnable(GL_DEPTH_TEST);

		glDisable(GL_CULL_FACE);
		// Shadow map		
		for (LightNode* lightNode : m_lightNodes)
		{
			if (ShadowMap* shadowMap = lightNode->shadowMap())
			{
				shadowMap->render_shadowMap(m_root_nodes);
				shadowMap->update_materials(m_materials);
			}
		}
		glEnable(GL_CULL_FACE);


		// Main render
		m_backbuffer_FBO.bind();

		glViewport(0, 0, m_dimensions.x, m_dimensions.y);
		m_cameraNode->update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (Material* material : m_materials)
		{
			material->update_lights(m_lightNodes);
		}

		for (auto const& node : m_root_nodes)
		{
			node.second->update_view(m_cameraNode);
			node.second->draw();
		}

		m_backbuffer_FBO.unbind();

		// Backbuffer render
		glViewport(0, 0, m_dimensions.x, m_dimensions.y);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glBindFramebuffer(GL_READ_FRAMEBUFFER, m_backbuffer_FBO.id());
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		//glBlitFramebuffer(
		//	0, 0, m_dimensions.x, m_dimensions.y,
		//	0, 0, m_dimensions.x, m_dimensions.y,
		//	GL_COLOR_BUFFER_BIT, GL_NEAREST
		//);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//m_backbuffer_color.bind();
		m_hdr_material.set_texture("hdr_buffer", &m_backbuffer_color);
		m_hdr_material.set_uniform("exposure", 1.3f);
		m_screen_node.draw();
		//m_hdr_material.set_texture("hdr_buffer", &m_backbuffer_depth);
		//m_screen_node.draw();



	}

	// // ----- GENERAL METHODS ----- // //
	void Renderer::init_settings()
	{
		/*

			!!! Eventually this should be abstracted out to be a part of the material settings

		*/

		// // Dark blue background // //
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		// // Enable depth test // //
		glEnable(GL_DEPTH_TEST);
		// // Enable backface culling // //
		glEnable(GL_CULL_FACE);
		// // Set winding direction // // 
		glFrontFace(GL_CCW);
		// // Accept fragment shader if it closer to the camera than the previous one
		glDepthFunc(GL_LESS);
		//glDepthFunc(GL_ALWAYS);
		// // Enable alpha
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// // Enable gamma correction
		//glEnable(GL_FRAMEBUFFER_SRGB);
	}

	void Renderer::init_first_frame()
	{
		GLuint num_pointLights = 0, num_directionalLights = 0, num_spotLights = 0;
		// Shadow map		
		for (LightNode* lightNode : m_lightNodes)
		{
			if (DirectionalLight* spotLight = dynamic_cast<DirectionalLight*> (lightNode->light()))
			{
				lightNode->set_shader_pos(num_directionalLights);
				num_directionalLights++;
			}
			if (PointLight* spotLight = dynamic_cast<PointLight*> (lightNode->light()))
			{
				lightNode->set_shader_pos(num_pointLights);
				num_pointLights++;
			}
			if (SpotLight* spotLight = dynamic_cast<SpotLight*> (lightNode->light()))
			{
				lightNode->set_shader_pos(num_spotLights);
				num_spotLights++;
			}
			if (ShadowMap* shadowMap = lightNode->shadowMap())
			{
				shadowMap->init_materials(m_materials);
			}
		}
	}

	void Renderer::init_hdr()
	{
		// Color backbuffer
		m_backbuffer_color.set_internal_format(GL_RGBA16F);
		m_backbuffer_color.set_width(m_dimensions.x);
		m_backbuffer_color.set_height(m_dimensions.y);
		m_backbuffer_color.set_format(GL_RGBA);
		m_backbuffer_color.set_type(GL_FLOAT);
		m_backbuffer_color.set_data(NULL);
		m_backbuffer_color.set_mipmap(true);
		m_backbuffer_color.set_min_filter(GL_NEAREST);
		m_backbuffer_color.set_mag_filter(GL_NEAREST);
		m_backbuffer_color.set_st_wrap(GL_CLAMP_TO_EDGE);
		m_backbuffer_color.process();

		// Depth backbuffer
		m_backbuffer_depth.set_internal_format(GL_DEPTH_COMPONENT);
		m_backbuffer_depth.set_width(m_dimensions.x);
		m_backbuffer_depth.set_height(m_dimensions.y);
		m_backbuffer_depth.set_format(GL_DEPTH_COMPONENT);
		m_backbuffer_depth.set_type(GL_FLOAT);
		m_backbuffer_depth.set_data(NULL);
		m_backbuffer_depth.set_mipmap(true);
		m_backbuffer_depth.set_min_filter(GL_NEAREST);
		m_backbuffer_depth.set_mag_filter(GL_NEAREST);
		m_backbuffer_depth.set_st_wrap(GL_CLAMP_TO_EDGE);
		m_backbuffer_depth.process();


		// Attach buffers
		m_backbuffer_FBO.bind();

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_backbuffer_color.id(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_backbuffer_depth.id(), 0);
		
		m_backbuffer_FBO.check_bound_framebuffer();
		m_backbuffer_FBO.unbind();
	}

	bool Renderer::poll_events()
	{
		SDL_Event loc_event;
		while (SDL_PollEvent(&loc_event))
		{
			if (loc_event.type == SDL_QUIT)
			{
				return false;
			}
		}
		return true;
	}


	void Renderer::update(Window * window, Timer * timer)
	{
		timer->update();
		render();
		window->finish_frame();
		window->append_title(("FPS: " + (std::string)timer->fps()));
	}

	// // ----- SETTERS ----- // //
	void Renderer::add_node(Node* node)
	{

		// Check if node exists in m_root_nodes
		if (m_root_nodes.find(node->name()) != m_root_nodes.end())
		{
			std::printf("WARNING: \"%s\" not added. Node with that name already exists\n", node->name().c_str());
			return;
		}

		// Add material from mesh node
		if (MeshNode* derived_meshNode = dynamic_cast<MeshNode*>(node))
		{
			add_material(derived_meshNode->material());
		}

		add_light_nodes(node);

		m_root_nodes[node->name()] = node;
	}

	void Renderer::add_light_nodes(Node* root_node)
	{
		if (LightNode* derived_lightNode = dynamic_cast<LightNode*>(root_node))
		{
			m_lightNodes.push_back(derived_lightNode);
		}
		for (auto child : root_node->children())
		{
			add_light_nodes(child.second);
		}
	}

	void Renderer::add_material(Material* material)
	{
		if (std::find(m_materials.begin(), m_materials.end(), material) == m_materials.end())
		{
			m_materials.push_back(material);
		}
	}
} // namespace gl_engine