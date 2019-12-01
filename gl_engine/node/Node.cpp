#include "Node.h"

#include <stack>

#include "../utils/VectorUtils.h"

Node::Node(const std::string name) : m_name(name)
{}

// // ----- GETTERS ----- // //
const glm::vec3& Node::position() const
{
	return m_position;
}

const glm::vec3& Node::rotation() const
{
	return m_rotation;
}

const glm::vec3& Node::scale() const
{
	return m_scale;
}

// // ----- SETTERS ----- // //
glm::vec3& Node::position()
{
	return m_position;
}

glm::vec3& Node::rotation()
{
	return m_rotation;
}

glm::vec3& Node::scale()
{
	return m_scale;
}

void Node::addChild(Node* child)
{
	child->addParent(this);
	m_children.push_back(child);
}

glm::mat4 Node::localTransform()
{
	glm::mat4 outMatrix = VectorUtils::trs(glm::mat3{ m_position, m_rotation, m_scale });
	return VectorUtils::trs(glm::mat3{ m_position, m_rotation, m_scale });
}

glm::mat4 Node::worldTransform()
{
	std::stack<glm::mat4> transforms;
	Node* currentNode = this;
	glm::mat4 outTransform(1.0f);

	// Collect all transforms
	while (currentNode != NULL)
	{
		transforms.push(currentNode->localTransform());
		currentNode = currentNode->m_parent;
	}
	
	// Apply transforms
	while (!transforms.empty())
	{
		outTransform *= transforms.top();
		transforms.pop();
	}
	return outTransform;
}

void Node::addParent(Node* parent)
{
	m_parent = parent;
}