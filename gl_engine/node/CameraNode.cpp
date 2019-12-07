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
const glm::vec3 CameraNode::worldPosition()
{
	glm::mat4 wTransform = worldTransform();
	return Camera::position() + glm::vec3(wTransform[3][0], wTransform[3][1], wTransform[3][2]);
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