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
#include "../shading/Material.h"
#include "../shading/ShadowMap.h"



namespace gl_engine
{
	// // ----- CONSTRUCTOR ----- // //
	Renderer::Renderer(CameraNode* cameraNode) :
		m_cameraNode(cameraNode), m_dimensions(cameraNode->camera()->dimensions())
	{
		m_cameraNode->camera()->setDimensions(m_dimensions);
		init_settings();
	}

	Renderer::Renderer(CameraNode* camera, const glm::uvec2& dimensions) :
		m_cameraNode(camera), m_dimensions(dimensions)
	{
		m_cameraNode->camera()->setDimensions(dimensions);
		init_settings();
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

	void Renderer::init_firstFrame()
	{
		// Shadow map		
		for (LightNode* lightNode : m_lightNodes)
		{
			if (ShadowMap* shadowMap = lightNode->shadowMap())
			{
				shadowMap->init_materials(m_materials);
			}
		}
	}

	//Draw all nodes to screen
	void Renderer::render()
	{
		if (m_firstFrame)
		{
			init_firstFrame();
			m_firstFrame = false;
		}
		
		// Shadow map		
		for (LightNode* lightNode : m_lightNodes)
		{
			if (ShadowMap* shadowMap = lightNode->shadowMap())
			{
				shadowMap->render_shadowMap(m_root_nodes);
				shadowMap->update_materials(m_materials);
			}
		}


		
		glViewport(0, 0, m_dimensions.x, m_dimensions.y);
		m_cameraNode->update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (Material* material : m_materials)
		{
			material->updateLights(m_lightNodes);
		}

		for (auto const& node : m_root_nodes)
		{
			node.second->update_view(m_cameraNode);
			node.second->draw();
		}
	}

	void Renderer::addNode(Node* node)
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
			addMaterial(derived_meshNode->material());
		}

		if (LightNode* derived_lightNode = dynamic_cast<LightNode*>(node))
		{
			addLightNode(derived_lightNode);
		}

		m_root_nodes[node->name()] = node;
	}

	void Renderer::addLightNode(LightNode* lightNode)
	{
		m_lightNodes.push_back(lightNode);
	}

	void Renderer::addMaterial(Material* material)
	{
		if (std::find(m_materials.begin(), m_materials.end(), material) == m_materials.end())
		{
			m_materials.push_back(material);
		}
	}

	bool Renderer::pollEvents()
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
		window->appendTitle(("FPS: " + (std::string)timer->fps()));
	}

} // namespace gl_engine