#include <stdio.h>

#include "Renderer.h"

#include "../node/MeshNode.h"


// // ----- CONSTRUCTOR ----- // //
Renderer::Renderer(CameraNode* cameraNode) : 
	m_cameraNode(cameraNode), m_dimensions(cameraNode->dimensions())
{
	m_cameraNode->setDimensions(m_dimensions);
	initSettings();
}

Renderer::Renderer(CameraNode* camera, const glm::uvec2& dimensions) :
	m_cameraNode(camera), m_dimensions(dimensions)
{
	m_cameraNode->setDimensions(dimensions);
	initSettings();
}

// // ----- GENERAL METHODS ----- // //
void Renderer::initSettings()
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
}


//Draw all nodes to screen
void Renderer::render()
{
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	 //Update object to perspective view buffer
	for (auto const& node : m_root_nodes)
	{
		node.second->update_view(m_cameraNode);
		if (MeshNode* derived_meshNode = dynamic_cast<MeshNode*>(node.second))
		{
			if (derived_meshNode->material()->containsUniform("light_position"))
			{
				glm::vec3 lightWorldPosition = m_lightNode->worldPosition();
				derived_meshNode->material()->setUniform("light_position", m_lightNode->worldPosition());
			}
		}
		node.second->draw();
	}
}


void Renderer::addNode(Node* node)
{
	auto search = m_root_nodes.find(node->name());
	if (m_root_nodes.find(node->name()) != m_root_nodes.end())
	{
		std::printf("WARNING: \"%s\" not added. Node with that name already exists\n", node->name().c_str());
		return;
	}
	m_root_nodes[node->name()] = node;
}

void Renderer::addLightNode(LightNode* lightNode)
{
	m_lightNode = lightNode;
	addNode(lightNode);
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
	m_cameraNode->update();
	render();
	window->finish_frame();
	window->appendTitle(("FPS: " + (std::string)timer->fps()));
}

// // ----- GETTERS ----- // //
//const CameraNode* camera() const;

// // ----- SETTERS ----- // //
//CameraNode* camera();