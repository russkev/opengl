#include <stdio.h>

#include "Renderer.h"

#include "../utils/Timer.h"
#include "../render/Window.h"
#include "../node/Node.h"
#include "../node/MeshNode.h"
#include "../node/CameraNode.h"
#include "../node/LightNode.h"
#include "../camera/Camera.h"
#include "../light/PointLight.h"
#include "../light/SpotLight.h"
#include "../light/DirectionalLight.h"
#include "../material/Material.h"
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

		render_shadow_maps();

		if (m_post_effects_enabled)
		{
			m_backbuffer_FBO.bind();

			render_geometry();

			m_backbuffer_FBO.unbind();

			clear_screen();
			m_backbuffer_FBO.bind();
			m_tone_map.draw();
			m_backbuffer_FBO.unbind();

			clear_screen();
			m_bloom.draw();
		}
		else
		{
			render_geometry();
		}

	}

	void Renderer::clear_screen()
	{
		glViewport(0, 0, m_dimensions.x, m_dimensions.y);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::render_shadow_maps()
	{
		if (m_shadow_maps.size() != 0)
		{
			glEnable(GL_DEPTH_TEST);

			glDisable(GL_CULL_FACE);

			for (ShadowMap* shadowMap : m_shadow_maps)
			{
				shadowMap->render_shadowMap(m_root_nodes);
				shadowMap->update_materials(m_materials);
			}
			glEnable(GL_CULL_FACE);
		}
	}

	void Renderer::render_geometry()
	{
		clear_screen();

		m_cameraNode->update();

		for (Material* material : m_materials)
		{
			material->update_lights(m_lightNodes);
		}

		for (auto const& node : m_root_nodes)
		{
			node.second->update_view(m_cameraNode);
			node.second->draw();
		}
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
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	void Renderer::init_deferred_renderer()
	{
		m_g_position	= Texture::create_16bit_rgb_null_texture(GL_TEXTURE_2D, &m_dimensions);
		m_g_normal		= Texture::create_16bit_rgb_null_texture(GL_TEXTURE_2D, &m_dimensions);
		m_g_color_spec	= Texture::create_8bit_rgba_null_texture(GL_TEXTURE_2D, &m_dimensions);
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
			if (ShadowMap* shadowMap = derived_lightNode->shadowMap())
			{
				m_shadow_maps.push_back(shadowMap);
			}

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

	void Renderer::enable_post_effects()
	{
		m_post_effects_enabled = true;
	}

	void Renderer::disable_post_effects()
	{
		m_post_effects_enabled = false;
	}
} // namespace gl_engine