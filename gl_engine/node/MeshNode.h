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
	inline static const std::string MODEL_TO_PROJECTION_UNIFORM_NAME = "mat_modelToProjection";
	static constexpr auto POSITION_ATTR = 0u;
	inline static const auto MESH_VAO_INFO = gl_introspect_tuple<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint, glm::vec3, glm::vec3>>::get();

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
	void updateView(Camera* camera) override;
	void draw() override;

};

#endif