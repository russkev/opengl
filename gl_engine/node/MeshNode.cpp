#include "MeshNode.h"

MeshNode::MeshNode(const std::string name, Mesh* mesh, Material* material) :
	Node(name), m_mesh(mesh), m_material(material)
{}
