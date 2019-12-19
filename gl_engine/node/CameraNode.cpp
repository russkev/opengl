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
	{
		m_camera->registerTransform(&Node::localTransform());
	}

	void CameraNode::update()
	{
		m_camera->update();
	}

	// // ----- GETTERS ----- // //
	Camera* CameraNode::camera()
	{
		return m_camera;
	}


	// // ----- OVERRIDES ----- // //
	const glm::vec3 CameraNode::worldPosition()
	{
		return glm::vec3(worldTransform() * glm::vec4(m_camera->position(), 1.0f));
	}

	void CameraNode::setPosition(const glm::vec3& position)
	{
		Node::setPosition(position);
	}

	// // ----- GETTERS ----- // //
	glm::mat4 CameraNode::worldToCam_matrix()
	{
		return  m_camera->objectToCam_matrix() * Node::worldTransform();
	}

	glm::mat4 CameraNode::worldToProjection_matrix()
	{
		return m_camera->transformToProjection_matrix(worldTransform());
	}
} // namespace gl_engine