#ifndef GL_ENGINE_NODE_MESH_H
#define GL_ENGINE_NODE_MESH_H

#include <glm/glm.hpp>

#include "Node.h"
#include "../mesh/Mesh.h"
#include "../shading/Material.h"
#include "../shading/Shader.h"
#include "../Buffer.h"
#include "CameraNode.h"
#include "../VAO.h"

/*

	Main mesh type node. Links a mesh to a material so it can be rendered on screen

*/
struct MeshNode : public Node
{
	static const std::string U_MODEL_TO_WORLD;
	// // ----- MEMBER VARIABLES ----- // //
private:
	Mesh* m_mesh;
	Material* m_material;
	Buffer m_vertexBuffer	= { GL_ARRAY_BUFFER, 0 };
	Buffer m_indexBuffer	= { GL_ARRAY_BUFFER, 0 };
	VAO m_vao;

	// // ----- CONSTRUCTOR ----- // //
public:
	MeshNode() {};
	MeshNode(const std::string name, Mesh* mesh, Material* material);

	// // ----- GENERAL METHODS ----- // //
	void update_view(Camera* camera) override;
	void draw() override;

	// // ----- GETTER ----- // //
	Mesh* mesh();
	Material* material();
};

#endif