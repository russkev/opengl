#pragma once

#include "Node.h"
#include "../mesh/Mesh.h"
#include "../shading/Material.h"
#include "../shading/Shader.h"

/*

	Main mesh type node. Links a mesh to a material so it can be rendered on screen

*/
struct MeshNode : public Node
{
	MeshNode(const std::string name, Mesh* mesh, Material* material);
private:
	Mesh* m_mesh;
	Material* m_material;
};