#ifndef GL_ENGINE_NODE_MESH_H
#define GL_ENGINE_NODE_MESH_H

#include <glm/glm.hpp>

#include "Node.h"
#include "../resources/Buffer.h"
#include "../resources/VAO.h"

namespace gl_engine
{
	// // ----- FORWARD DECLARATION ----- // //
	struct Mesh;
	struct Material;
	struct Shader;
	struct CameraNode;

	/*

		Main mesh type node. Links a mesh to a material so it can be rendered on screen

	*/
	struct MeshNode : public Node
	{
		// // ----- CONSTANTS ----- // //
		static const std::string U_MODEL_TO_WORLD;
		static const std::string U_MODEL_TO_WORLD_NORMAL;
		static const std::string U_WORLD_TO_CAM;
		static const std::string U_CAM;

		// // ----- CONSTRUCTOR ----- // //
	public:
		MeshNode() {};
		MeshNode(const std::string name, Mesh* mesh, Material* material);

		// // ----- GENERAL METHODS ----- // //
		void update_view(CameraNode* cameraNode) override;
		void draw(const Pass& pass = rgb) override;
		void draw_material(Material* material) override;

		// // ----- GETTERS ----- // //
		Mesh* mesh();
		Material* material();

		// // ----- SETTERS ----- // //

		// // ----- MEMBER VARIABLES ----- // //
	private:
		Mesh* m_mesh;
		Material* m_material;
		Buffer m_vertex_buffer = { GL_ARRAY_BUFFER, 0 };
		Buffer m_index_buffer = { GL_ARRAY_BUFFER, 0 };
		VAO m_vao;
	};
} // namespace gl_engine
#endif