#include "Node.h"

#include "../utils/VectorUtils.h"
#include "CameraNode.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>


namespace gl_engine
{
	const std::string Node::U_MODEL_TO_PROJECTION = "transform.model_to_projection";
	const glm::vec3 Node::FORWARD_DIRECTION = { 0.0, 0.0, 1.0 };


	Node::Node(const std::string name) : m_name(name)
	{}


	// Add new child to list of children in the node
	void Node::add_child(Node* child)
	{
		if (m_children.find(child->m_name) == m_children.end())
		{
			m_children[child->m_name] = child;
			child->addParent(this);
		}
		else
		{
			throw std::runtime_error("Unable to add " + child->m_name + ", node with that name already exists");
		}
	}


	// Change the existing parent. All children will come along.
	void Node::set_parent(Node* parent)
	{
		m_parent = parent;
		parent->add_child(this);
	}

	// Disconnect child and return a pointer to that child. 
	Node* Node::disconnect_child(const std::string child_name)
	{
		if (m_children.find(child_name) != m_children.end())
		{
			Node* child = m_children[child_name];
			m_children.erase(child_name);
			child->m_parent = NULL;
			return child;
		}
		return NULL;
	}

	// Calculate the transform matrix in local space
	glm::mat4& Node::local_to_node()
	{
		return m_local_transform;
	}

	// Calculate the transform matrix in world space
	const glm::mat4 Node::world_to_node()
	{
		Node* currentNode = this;
		glm::mat4 outTransform(1.0f);

		while (currentNode != NULL)
		{
			outTransform = currentNode->local_to_node() * outTransform;
			currentNode = currentNode->m_parent;
		}

		return outTransform;
	}

	const glm::mat3 Node::world_normal_to_node()
	{
		return glm::mat3(glm::transpose(glm::inverse(world_to_node())));
	}

	const glm::vec3 Node::direction()
	{
		return glm::mat3(world_to_node()) * FORWARD_DIRECTION;
	}

	void Node::addParent(Node* parent)
	{
		m_parent = parent;
	}

	void Node::update_view(CameraNode* camera_node)
	{
		for (auto child : m_children)
		{
			child.second->update_view(camera_node);
		}
	}
	void Node::draw(const Pass& pass)
	{
		for (auto child : m_children)
		{
			child.second->draw();
		}
	}

	// // ----- GETTERS ----- // //
	const std::string& Node::name() const
	{
		return m_name;
	}


	const glm::vec3 Node::local_position() const
	{
		return VectorUtils::extract_position(m_local_transform);
	}

	const glm::vec3 Node::world_position()
	{
		return VectorUtils::extract_position(world_to_node());
	}

	const glm::vec3 Node::local_rotation() const
	{
		return m_rotation;
	}

	const glm::vec3 Node::local_scale() const
	{
		glm::vec3 out_scale;

		glm::decompose(m_local_transform, out_scale, glm::quat{}, glm::vec3{}, glm::vec3{}, glm::vec4{});

		return out_scale;
	}

	const Node* Node::parent() const
	{
		return m_parent;
	}

	std::unordered_map<std::string, Node*>& Node::children()
	{
		return m_children;
	}

	// // ----- SETTERS ----- // //
	void Node::set_position(const glm::vec3& position)
	{
		m_local_transform[3] = glm::vec4(position, 1.0f);
	}

	void Node::set_rotation(const glm::vec3& rotation)
	{
		m_rotation = rotation;
		m_local_transform =
			glm::translate(glm::mat4{ 1.0f }, local_position()) *
			glm::yawPitchRoll(glm::radians(m_rotation.y), glm::radians(m_rotation.x), glm::radians(m_rotation.z)) *
			glm::scale(glm::mat4{ 1.0f }, local_scale());
	}

	void Node::set_scale(const glm::vec3& scale)
	{
		glm::vec3 existing_rotation = local_rotation();
		m_local_transform =
			glm::translate(glm::mat4{ 1.0f }, local_position()) *
			glm::yawPitchRoll(existing_rotation.y, existing_rotation.x, existing_rotation.z) *
			glm::scale(glm::mat4{ 1.0 }, scale);
	}

	void Node::set_scale(const GLfloat scale)
	{
		set_scale(glm::vec3{ scale, scale, scale });
	}
} // namespace gl_engine