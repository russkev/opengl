#include "Node.h"

#include "../utils/VectorUtils.h"
#include "CameraNode.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>


namespace gl_engine
{
	const std::string Node::U_MODEL_TO_PROJECTION = "transform.modelToProjection";
	const glm::vec3 Node::FORWARD_DIRECTION = { 0.0, 0.0, 1.0 };


	Node::Node(const std::string name) : m_name(name)
	{}


	// Add new child to list of children in the node
	void Node::addChild(Node* child)
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
	void Node::setParent(Node* parent)
	{
		m_parent = parent;
		parent->addChild(this);
	}

	// Disconnect child and return a pointer to that child. 
	Node* Node::disconnectChild(const std::string childName)
	{
		if (m_children.find(childName) != m_children.end())
		{
			Node* child = m_children[childName];
			m_children.erase(childName);
			child->m_parent = NULL;
			return child;
		}
		return NULL;
	}

	// Calculate the transform matrix in local space
	glm::mat4& Node::localTransform()
	{
		return m_local_transform;
		//return VectorUtils::trs(glm::mat3{ m_position, m_rotation, m_scale });
	}

	// Calculate the transform matrix in world space
	const glm::mat4 Node::worldTransform()
	{
		Node* currentNode = this;
		glm::mat4 outTransform(1.0f);

		while (currentNode != NULL)
		{
			outTransform = currentNode->localTransform() * outTransform;
			currentNode = currentNode->m_parent;
		}

		return outTransform;
	}

	const glm::mat3 Node::worldNormalTransform()
	{
		return glm::mat3(glm::transpose(glm::inverse(worldTransform())));
	}

	const glm::vec3 Node::directionVector()
	{
		return glm::mat3(worldTransform()) * FORWARD_DIRECTION;
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
	void Node::draw()
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


	const glm::vec3 Node::position() const
	{
		return glm::vec3(m_local_transform * glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
		//return m_position;
	}

	const glm::vec3 Node::worldPosition()
	{
		return glm::vec3(worldTransform() * glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
	}

	const glm::vec3 Node::rotation() const
	{
		glm::vec3 out_rotation;
		glm::quat temp_quat{};


		glm::decompose(m_local_transform, glm::vec3{}, temp_quat, glm::vec3{}, glm::vec3{}, glm::vec4{});
		out_rotation = glm::eulerAngles(temp_quat);

		return glm::degrees(out_rotation);
	}

	const glm::vec3 Node::scale() const
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
	void Node::setPosition(const glm::vec3& position)
	{
		m_local_transform[3] = glm::vec4(position, 1.0f);
		//m_position = position;
	}

	void Node::setRotation(const glm::vec3& rotation)
	{
		m_local_transform =
			glm::translate(glm::mat4{ 1.0f }, position()) *
			glm::yawPitchRoll(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z)) *
			glm::scale(glm::mat4{ 1.0f }, scale());
	}

	void Node::setScale(const glm::vec3& scale)
	{
		glm::vec3 existing_rotation = rotation();
		m_local_transform =
			glm::translate(glm::mat4{ 1.0f }, position()) *
			glm::yawPitchRoll(existing_rotation.y, existing_rotation.x, existing_rotation.z) *
			glm::scale(glm::mat4{ 1.0 }, scale);
		//m_scale = scale;
	}

	void Node::setScale(const GLfloat scale)
	{
		setScale(glm::vec3{ scale, scale, scale });
		//m_scale = { scale, scale, scale };
	}
} // namespace gl_engine