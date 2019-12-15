#include "CameraNode.h"

#include "../camera/Camera.h"

namespace gl_engine
{
	/*

		Constructor. Requires name for the node and a pointer to a camera

	*/
	CameraNode::CameraNode(const std::string name, Camera* camera) :
		Node(name), m_camera(camera)
	{}

	// // ----- GETTERS ----- // //
	Camera* CameraNode::camera()
	{
		return m_camera;
	}


	// // ----- OVERRIDES ----- // //
	const glm::vec3 CameraNode::position() const
	{
		return glm::vec3(m_camera->position() + Node::position());
	}
	const glm::vec3 CameraNode::worldPosition()
	{
		glm::mat4 wTransform = worldTransform();
		return m_camera->position() + glm::vec3(wTransform[3][0], wTransform[3][1], wTransform[3][2]);
	}

	void CameraNode::setPosition(const glm::vec3& position)
	{
		Node::setPosition(position);
	}

	glm::mat4 CameraNode::worldToCam_matrix()
	{
		return  m_camera->worldToCam_matrix() * Node::worldTransform();
	}

	glm::mat4 CameraNode::worldToProjection_matrix()
	{
		return  m_camera->worldToProjection_matrix() * Node::worldTransform();
	}
} // namespace gl_engine