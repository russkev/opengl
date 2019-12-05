#include "Node.h"

#include <stack>

#include "../utils/VectorUtils.h"

Node::Node(const std::string name) : m_name(name)
{}


/*

	Add new child to list of children in the node

*/
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


/*

	Change the existing parent. All children will come along.

*/
void Node::setParent(Node* parent)
{
	m_parent = parent;
}

/*

	Disconnect child and return a pointer to that child. 
	The children of the child will stay connected to the child

*/
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

/*

	Calculate the transform matrix in local space

*/
glm::mat4 Node::localTransform()
{
	return VectorUtils::trs(glm::mat3{ m_position, m_rotation, m_scale });
}

/*

	Calculate the transform matrix in world space

*/
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

// // ----- GETTERS ----- // //
const std::string& Node::name() const
{
	return m_name;
}


const glm::vec3 Node::position() const
{
	return m_position;
}

const glm::vec3 Node::rotation() const
{
	return m_rotation;
}

const glm::vec3 Node::scale() const
{
	return m_scale;
}

const Node* Node::parent() const
{
	return m_parent;
}

const std::unordered_map<std::string, Node*> Node::children() const
{
	return m_children;
}

// // ----- SETTERS ----- // //
void Node::setPosition(const glm::vec3& position)
{
	m_position = position;
}

void Node::setRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
}

void Node::sestScale(const glm::vec3& scale)
{
	m_scale = scale;
}