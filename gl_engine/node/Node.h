#ifndef GL_ENGINE_NODE_NODE_H
#define GL_ENGINE_NODE_NODE_H

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

/*
	
	Base node class. All other nodes derive from this. It can also be used as a transform node itself.

*/
struct Node
{
	// // ----- CONSTRUCTOR ----- // //
	Node(const std::string name);


	void addChild(Node* child);
	void setParent(Node* parent);
	Node* disconnectChild(const std::string nodeName);
	glm::mat4 localTransform();
	glm::mat4 worldTransform();
	void updateView(void*) {};


	// // ----- GETTERS ----- // //
	const glm::vec3& position() const;
	const glm::vec3& rotation() const;
	const glm::vec3& scale() const;
	const Node* parent() const;
	const std::unordered_map<std::string, Node*> children() const;

	// // ----- SETTERS ----- // //
	glm::vec3& position();
	glm::vec3& rotation();
	glm::vec3& scale();


private:
	void addParent(Node* parent);

private:
	std::string m_name;
	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };

	Node* m_parent = NULL;
	std::unordered_map<std::string, Node*> m_children;
};

#endif