#include "Renderer.h"

/*

	Constructor

*/
Renderer::Renderer(CameraNode* camera) : 
	m_camera_node(camera)
{}

Renderer::Renderer(CameraNode* camera, const glm::ivec2& dimensions) :
	m_camera_node(camera), m_dimensions(dimensions)
{
	m_camera_node->camera()->setDimensions(dimensions);
}

/*

	Draw all nodes to screen

*/
void Renderer::render()
{
	// Update object to perspective view buffer
	//for (auto const& node : m_root_nodes)
	//{
	//	node.second->updateView(m_camera, m_dimensions);
	//}
}

// // ----- GENERAL METHODS ----- // //
//void addNode(Node* node);

// // ----- GETTERS ----- // //
//const CameraNode* camera() const;

// // ----- SETTERS ----- // //
//CameraNode* camera();