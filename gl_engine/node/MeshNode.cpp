#ifndef GL_ENGINE_NODE_MESH_H
#define GL_ENGINE_NODE_MESH_H

#include "MeshNode.h"

/*

	Constructor
	Set member variables and upload mesh vertices and indices buffers

*/
MeshNode::MeshNode(const std::string name, Mesh* mesh, Material* material) :
	Node(name), m_mesh(mesh), m_material(material)
{
	m_vertexBuffer.append(m_mesh->vertices());
	m_indexBuffer.append(m_mesh->indices());
}

//void MeshNode::updateView(CameraNode* camera)
//{
//	//
//}

#endif