#ifndef GL_ENGINE_NODE_MESH_H
#define GL_ENGINE_NODE_MESH_H

#include "Node.h"
#include "resources/Buffer.h"
#include "resources/VAO.h"

namespace glen
{
	// // ----- FORWARD DECLARATION ----- // //
	struct Mesh;
	struct Material;
	struct Material;
	struct CameraNode;

	/*!

		Main mesh type node. 
		Links a mesh to a material so it can be rendered on screen

	*/
	struct MeshNode : public Node
	{
		// @cond

		// // ----- CONSTANTS ----- // //
		static const std::string U_MODEL_TO_WORLD;
		static const std::string U_MODEL_TO_WORLD_NORMAL;
		static const std::string U_WORLD_TO_CAM;
		static const std::string U_CAM;

		// @endcond

		// // ----- CONSTRUCTOR ----- // //
	public:
		/*!
		 * @brief Constructor. Name, `mesh` and `material` are required
		 * to create a Mesh Node.
		*/
		MeshNode(const std::string name, Mesh* mesh, Material* material);

		/*!
		 * @brief Use move constructor instead of copy constructor
		*/
		MeshNode(const MeshNode& other) = delete;

		/*!
		 * @brief Move constructor
		*/
		MeshNode(MeshNode&& other) = default;

		/*!
		 * @brief Use move assign instead of copy assign
		*/
		MeshNode& operator = (const MeshNode& other) = delete;

		/*!
		 * @brief Move assign
		*/
		MeshNode& operator = (MeshNode&& other) = default;

		~MeshNode() = default;

		// // ----- GENERAL METHODS ----- // //
		void update_view(CameraNode* cameraNode) override;
		
		/*!
		 * @brief Render the mesh with assigned material applied
		*/
		void draw() override;

		/*!
		 * @brief Render mesh with provided `material` applied
		*/
		void draw_material(Material* material) override;

		
		// // ----- GETTERS ----- // //

		/*!
		 * @return The mesh object (mutable)
		*/
		Mesh* mesh();

		/*!
		 * @return The assigned material (mutable)
		*/
		Material* material();

		// // ----- SETTERS ----- // //
		void set_material(Material* material);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		Mesh* m_mesh;
		Material* m_material;
		Buffer m_vertex_buffer = { GL_ARRAY_BUFFER, 0 };
		Buffer m_index_buffer = { GL_ARRAY_BUFFER, 0 };
		VAO m_vao;
	};
}
#endif