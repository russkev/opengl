#pragma once

#include "Node.h";
#include "../mesh/Mesh.h"
#include "../shading/Material.h"

struct MeshNode : public Node
{
private:
	Mesh* m_mesh;
	Material* material;
};