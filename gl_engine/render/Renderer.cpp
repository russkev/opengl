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
	 //Update object to perspective view buffer
	for (auto const& node : m_root_nodes)
	{

		node.second
		node.second->updateVew(m_camera_node);
	}
}

// // ----- GENERAL METHODS ----- // //
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

// // ----- GETTERS ----- // //
//const CameraNode* camera() const;

// // ----- SETTERS ----- // //
//CameraNode* camera();