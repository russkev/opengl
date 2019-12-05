#include "CameraNode.h"

/*

	Constructor. Requires name for the node and a pointer to a camera

*/
CameraNode::CameraNode(const std::string name) :
	Node(name), Camera()
{}

// // ----- SETTERS ----- // //
//Camera* CameraNode::camera()
//{
//	return m_camera;
//}


// // ----- GETTERS ----- // //
//const std::string& CameraNode::name() const
//{
//	return m_name;
//}
//const Camera* CameraNode::camera() const
//{
//	return m_camera;
//}


// // ----- OVERRIDES ----- // //
const glm::vec3 CameraNode::position() const
{
	return glm::vec3(Camera::position() + Node::position());
}

void CameraNode::setPosition(const glm::vec3& position)
{
	Node::setPosition(position);
}

glm::mat4 CameraNode::worldToViewMatrix()
{
	return Camera::worldToViewMatrix() * Node::worldTransform();
}

glm::mat4 CameraNode::worldToProjectionMatrix()
{
	return Camera::worldToProjectionMatrix() * Node::worldTransform();
}