#include "pch.h"
#include "MeshNode.h"

#include "CameraNode.h"
#include "mesh/Mesh.h"
#include "material/Material.h"

namespace glen
{
	// // ----- CONSTANTS ----- // //
	const std::string MeshNode::U_MODEL_TO_WORLD = "transform.model_to_world";
	const std::string MeshNode::U_MODEL_TO_WORLD_NORMAL = "transform.model_to_world_normal";
	const std::string MeshNode::U_CAM = "camera.position";


	// // ----- CONSTRUCTORS ----- // //
	MeshNode::MeshNode(const std::string name, Mesh* mesh, Material* material) :
		Node(name), m_mesh(mesh), m_material(material)
	{
		m_vertex_buffer.append(*m_mesh->vertices());
		m_index_buffer.append(*m_mesh->indices());

		// // Upload the VAO information
		m_vao.generate_VAO(m_vertex_buffer, 0, MESH_VAO_INFO.data(), MESH_VAO_INFO.data() + MESH_VAO_INFO.size(), POSITION_ATTR);
	}


	// // ----- GENERAL METHODS ----- // //
	void MeshNode::update_view(CameraNode* cameraNode)
	{
		glm::mat4 modelToPerspective_matrix = cameraNode->world_to_projection() * Node::world_to_node();
		m_material->update_view(cameraNode, this);
		Node::update_view(cameraNode);
	}

	void MeshNode::draw()
	{
		m_material->use();
		m_vao.bind();
		m_index_buffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		m_material->bind_textures();

		glDrawElements(GL_TRIANGLES, (GLsizei)m_index_buffer.size(), GL_UNSIGNED_SHORT, 0);

		m_material->unbind_textures();

		for (auto child : Node::children())
		{
			child.second->draw();
		}


	}

	void MeshNode::draw_material(Material* material)
	{
		material->use();
		m_vao.bind();
		m_index_buffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		material->bind_textures();

		glDrawElements(GL_TRIANGLES, (GLsizei)m_index_buffer.size(), GL_UNSIGNED_SHORT, 0);

		material->unbind_textures();

		for (auto child : Node::children())
		{
			child.second->draw_material(material);
		}
	}


	// // ----- GETTERS ----- // //
	Mesh* MeshNode::mesh()
	{
		return m_mesh;
	}

	Material* MeshNode::material()
	{
		return m_material;
	}

	// // ----- SETTERS ----- // //
	void MeshNode::set_material(Material* material)
	{
		m_material = material;
	}
} 


