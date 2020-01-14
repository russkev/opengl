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


			/*

			Test

			*/

			//m_test_screen_material.set_texture("screen_texture", &m_test_texture);
			//m_test_hdr_material.set_texture("hdr_buffer", &m_test_texture);

			/*

			End test

			*/

		}

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

		m_hdr_framebuffer.bind();

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

		m_hdr_framebuffer.unbind();
		glViewport(0, 0, m_dimensions.x, m_dimensions.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//m_hdr_material.set_texture("hdr_buffer", &m_hdr_texture);
		//m_hdr_material.set_uniform("exposure", 1.0);
		//m_screen_node.draw();

		/*

		Test

		*/

		//glDisable(GL_BLEND);

		//m_hdr_texture.bind();

		//m_test_hdr_material.use();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m_hdr_texture.id);

		//m_test_hdr_material.set_texture("hdr_buffer", &m_hdr_texture);

		//m_test_screen_node.draw();

		//m_hdr_material.use();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m_hdr_texture.id());

		//m_hdr_material.set_texture("hdr_buffer", &m_test_texture);
		m_hdr_material.set_texture("hdr_buffer", &m_hdr_texture);

		m_screen_node.draw();

		//m_hdr_texture.unbind();

		glEnable(GL_BLEND);




		/*

		End test

		*/

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
		glEnable(GL_FRAMEBUFFER_SRGB);
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
		m_hdr_texture.set_internal_format(GL_RGBA16F);
		m_hdr_texture.set_width(m_dimensions.x);
		m_hdr_texture.set_height(m_dimensions.y);
		m_hdr_texture.set_format(GL_RGBA);
		m_hdr_texture.set_type(GL_FLOAT);
		m_hdr_texture.set_data(NULL);
		m_hdr_texture.set_mipmap(false);
		m_hdr_texture.set_min_filter(GL_NEAREST);
		m_hdr_texture.set_mag_filter(GL_NEAREST);
		m_hdr_texture.set_st_wrap(GL_CLAMP_TO_EDGE);
		m_hdr_texture.process();


		m_hdr_texture.bind();
		// Create depth buffer
		glGenRenderbuffers(1, &m_rbo_depth_id);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo_depth_id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_dimensions.x, m_dimensions.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo_depth_id);


		// Attach buffers
		m_hdr_framebuffer.bind();
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hdr_texture.id(), 0);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo_depth_id);
		
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_hdr_texture.id(), 0);

		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);
		
		
		m_hdr_framebuffer.check_bound_framebuffer();
		m_hdr_framebuffer.unbind();
		m_hdr_texture.unbind();
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