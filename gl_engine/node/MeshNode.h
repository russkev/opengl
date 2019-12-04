#pragma once

#include <glm/glm.hpp>

#include "Node.h"
#include "../mesh/Mesh.h"
#include "../shading/Material.h"
#include "../shading/Shader.h"
#include "../Buffer.h"

/*

	Main mesh type node. Links a mesh to a material so it can be rendered on screen

*/
struct MeshNode : public Node
{
	// // ----- MEMBER VARIABLES ----- // //
private:
	Mesh* m_mesh;
	Material* m_material;
	Buffer m_vertexBuffer	= { GL_ARRAY_BUFFER, 0 };
	Buffer m_indexBuffer	= { GL_ARRAY_BUFFER, 0 };
	glm::mat4 m_viewMatrixBuffer = glm::mat4(1.0f);


	// // ----- CONSTRUCTOR ----- // //
public:
	MeshNode(const std::string name, Mesh* mesh, Material* material);


	//void updateView(CameraNode* camera);

};