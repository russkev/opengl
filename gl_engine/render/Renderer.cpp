#include <stdio.h>

#include "Renderer.h"

#include "../node/MeshNode.h"


// // ----- CONSTRUCTOR ----- // //
Renderer::Renderer(CameraNode* cameraNode) : 
	m_cameraNode(cameraNode), m_dimensions(cameraNode->dimensions())
{
	initSettings();
}

Renderer::Renderer(CameraNode* camera, const glm::uvec2& dimensions) :
	m_cameraNode(camera), m_dimensions(dimensions), m_timer(Timer())
{
	m_cameraNode->setDimensions(dimensions);
	initSettings();
}

// // ----- GENERAL METHODS ----- // //
void Renderer::initSettings()
{
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
		node.second->updateView(m_cameraNode);
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

void Renderer::go(Window* window)
{

	while (pollEvents())
	{
		/*
		
			Animation section

		*/
		Node* arrowNode = m_root_nodes["Arrow1"];

		float new_y_rotation = arrowNode->rotation().y + (float)m_timer.delta_time_s() * 10;
		arrowNode->setRotation({ 0.0, new_y_rotation, 0.0 });

		glm::vec4 newColor = { 0.3, (std::cos(m_timer.total_time_s() * 5) + 1) / 2, 0.8, 1.0 };
		MeshNode *arrowNode2 = (MeshNode*)arrowNode;
		arrowNode2->material()->setUniform("uColor", newColor);


		/*
		
			Normal update section

		*/
		m_timer.update();
		m_cameraNode->update();
		render();
		window->finish_frame();
		window->appendTitle(("FPS: " + (std::string)m_timer.fps()));
	}
}

// // ----- GETTERS ----- // //
//const CameraNode* camera() const;

// // ----- SETTERS ----- // //
//CameraNode* camera();