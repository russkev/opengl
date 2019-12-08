#include "Node.h"

#include <stack>

#include "../utils/VectorUtils.h"


const std::string Node::U_MODEL_TO_PROJECTION = "transform.modelToProjection";


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
const glm::mat4 Node::localTransform()
{
	return VectorUtils::trs(glm::mat3{ m_position, m_rotation, m_scale });
}

// Calculate the transform matrix in world space
const glm::mat4 Node::worldTransform()
{
	//return localTransform();

	std::stack<glm::mat4> transforms;
	Node* currentNode = this;
	glm::mat4 outTransform(1.0f);

	// Collect all transforms
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

const glm::vec3 Node::worldPosition()
{
	glm::mat4 wTransform = worldTransform();
	return glm::vec3(wTransform[3][0], wTransform[3][1], wTransform[3][2]);
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

std::unordered_map<std::string, Node*>& Node::children()
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

void Node::setScale(const glm::vec3& scale)
{
	m_scale = scale;
}

void Node::setScale(const GLfloat scale)
{
	m_scale = { scale, scale, scale };
}