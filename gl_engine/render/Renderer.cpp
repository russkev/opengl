#include "Renderer.h"

/*

	Constructor

*/
Renderer::Renderer(CameraNode* cameraNode) : 
	m_camera_node(cameraNode), m_dimensions(cameraNode->camera()->dimensions())
{}

Renderer::Renderer(CameraNode* camera, const glm::uvec2& dimensions) :
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