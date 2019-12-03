#pragma once

#include "Node.h"
#include "../mesh/Mesh.h"
#include "../shading/Material.h"

struct MeshNode : public Node
{
	MeshNode(const std::string name, Mesh* mesh, Material* material);
private:
	Mesh* m_mesh;
	Material* m_material;
};