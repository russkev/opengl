#include "CameraNode.h"

/*

	Constructor. Requires name for the node and a pointer to a camera

*/
CameraNode::CameraNode(const std::string name, Camera* camera) :
	Node(name), m_camera(camera)
{}

// // ----- SETTERS ----- // //
Camera* CameraNode::camera()
{
	return m_camera;
}


// // ----- GETTERS ----- // //
const std::string& CameraNode::name() const
{
	return m_name;
}
const Camera* CameraNode::camera() const
{
	return m_camera;
}