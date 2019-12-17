#include "CameraNode.h"

#include "../camera/Camera.h"
#include "../utils/VectorUtils.h"

namespace gl_engine
{
	/*

		Constructor. Requires name for the node and a pointer to a camera

	*/
	CameraNode::CameraNode(const std::string name, Camera* camera) :
		Node(name), m_camera(camera)
	{}

	void CameraNode::update()
	{
		m_camera->update(&Node::localTransform());
	}

	// // ----- GETTERS ----- // //
	Camera* CameraNode::camera()
	{
		return m_camera;
	}


	// // ----- OVERRIDES ----- // //
	//const glm::mat4 CameraNode::localTransform()
	//{
	//	return Node::localTransform();// *glm::translate(glm::mat4(1.0f), m_camera->position());
	//}


	const glm::vec3 CameraNode::position() const
	{
		return Node::position();
		//return glm::vec3(m_camera->position() + Node::position());
	}
	const glm::vec3 CameraNode::worldPosition()
	{
		//return glm::vec3(worldTransform() * glm::vec4(0.0, 0.0, 0.0, 1.0));
		return glm::vec3(worldTransform() * glm::vec4(m_camera->position(), 1.0f));
	}

	void CameraNode::setPosition(const glm::vec3& position)
	{
		Node::setPosition(position);
	}

	// // ----- GETTERS ----- // //
	glm::mat4 CameraNode::objectToCam_matrix()
	{
		//return m_camera->objectToCam_matrix();
		return  m_camera->objectToCam_matrix() * Node::worldTransform();
	}

	glm::mat4 CameraNode::worldToProjection_matrix()
	{
		auto sdkjf = Node::worldTransform();
		return m_camera->worldToProjection_matrix();// *Node::worldTransform();
		//return m_camera->viewToProjection_matrix();
		//return m_camera->viewToProjectionMatrix() * objectToCam_matrix();
		//return  m_camera->worldToProjection_matrix();// *objectToCam_matrix();//Node::worldTransform();
	}
} // namespace gl_engine