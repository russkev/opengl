#include "MeshNode.h"

#include "../mesh/Mesh.h"
#include "../shading/Material.h"
#include "../shading/Shader.h"
#include "CameraNode.h"

namespace gl_engine
{
	const std::string MeshNode::U_MODEL_TO_WORLD = "transform.model_to_world";
	const std::string MeshNode::U_MODEL_TO_WORLD_NORMAL = "transform.model_to_world_normal";
	const std::string MeshNode::U_WORLD_TO_CAM = "transform.world_to_cam";
	const std::string MeshNode::U_CAM = "camera.position";


	//Set member variables and upload mesh vertices and indices buffers
	MeshNode::MeshNode(const std::string name, Mesh* mesh, Material* material) :
		Node(name), m_mesh(mesh), m_material(material)
	{
		m_vertex_buffer.append(m_mesh->vertices());
		m_index_buffer.append(m_mesh->indices());

		// // Upload the VAO information
		m_vao.generate_VAO(m_vertex_buffer, 0, MESH_VAO_INFO.data(), MESH_VAO_INFO.data() + MESH_VAO_INFO.size(), POSITION_ATTR);
	}


	void MeshNode::update_view(CameraNode* cameraNode)
	{
		glm::mat4 modelToPerspective_matrix = cameraNode->world_to_projection() * Node::world_to_node();

		m_material->set_uniform(U_MODEL_TO_PROJECTION, modelToPerspective_matrix);
		m_material->set_uniform(U_MODEL_TO_WORLD, Node::world_to_node());
		m_material->set_uniform(U_MODEL_TO_WORLD_NORMAL, Node::world_normal_to_node());
		m_material->set_uniform(U_WORLD_TO_CAM, cameraNode->world_to_cam());
		m_material->set_uniform(U_CAM, cameraNode->world_position());

		for (auto child : Node::children())
		{
			child.second->update_view(cameraNode);
		}

	}

	void MeshNode::draw(const Pass& pass)
	{

		draw_material(m_material);

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
	}


	Mesh* MeshNode::mesh()
	{
		return m_mesh;
	}

	Material* MeshNode::material()
	{
		return m_material;
	}
} // namespace gl_engine



