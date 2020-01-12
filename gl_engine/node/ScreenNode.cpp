#include "ScreenNode.h"

#include "../mesh/Mesh.h"
#include "../shading/Material.h"

namespace gl_engine
{
	// // ----- CONSTRUCTORS ----- // //
	ScreenNode::ScreenNode(const std::string name, Mesh* mesh, Material* material) :
		Node{ name }, m_mesh{ mesh }, m_material{ material }
	{
		m_vertex_buffer.append(m_mesh->vertices());
		m_index_buffer.append(m_mesh->indices());
		m_vao.generate_VAO(m_vertex_buffer, 0, MESH_VAO_INFO.data(), MESH_VAO_INFO.data() + MESH_VAO_INFO.size(), POSITION_ATTR);
	}

	// // ----- GENERAL METHODS ----- // //
	void ScreenNode::draw(const Pass& pass)
	{
		m_material->use();
		m_vao.bind();
		m_index_buffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		m_material->bind_textures();

		glDrawElements(GL_TRIANGLES, (GLsizei)m_index_buffer.size(), GL_UNSIGNED_SHORT, 0);

		m_material->unbind_textures();

		{
			for (auto child : Node::children())
			{
				child.second->draw();
			}
		}
	}

	void ScreenNode::draw_material(Material* material)
	{
		material->use();
		m_vao.bind();
		m_index_buffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		material->bind_textures();

		glDrawElements(GL_TRIANGLES, (GLsizei)m_index_buffer.size(), GL_UNSIGNED_SHORT, 0);

		material->unbind_textures();

		{
			for (auto child : Node::children())
			{
				child.second->draw_material(material);
			}
		}

	}

	// // ----- GETTERS ----- // //
	const glm::vec3 ScreenNode::local_position() const
	{
		glm::vec3 position{ Node::local_position() };
		return glm::vec3{ position.x, position.y, 0.0f };
	}

	const glm::vec3 ScreenNode::world_position()
	{
		glm::vec3 position{ Node::world_position() };
		return glm::vec3{ position.x, position.y, 0.0f };
	}

	// // ----- SETTERS ----- // //
	void ScreenNode::set_position(const glm::vec3& position)
	{
		set_position(glm::vec2{ position.x, position.y });
	}

	void ScreenNode::set_position(const glm::vec2& position)
	{
		Node::set_position( glm::vec3{position.x, position.y, 0.0} );
	}

	void ScreenNode::set_rotation(const glm::vec3& rotation)
	{
		set_rotation(glm::vec2{ rotation.x, rotation.y });
	}

	void ScreenNode::set_rotation(const glm::vec2& rotation)
	{
		Node::set_rotation(glm::vec3{ rotation.x, rotation.y, 0.0 });
	}

	void ScreenNode::set_scale(const glm::vec3& scale)
	{
		set_scale(glm::vec2{ scale.x, scale.y });
	}

	void ScreenNode::set_scale(const glm::vec2& scale)
	{
		Node::set_scale(glm::vec3{ scale.x, scale.y, 0.0 });
	}

	void ScreenNode::set_scale(const GLfloat scale)
	{
		set_scale(glm::vec3{ scale, scale, 0.0 });
	}

} // namespace gl_engine

