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
		m_camera->register_transform(&Node::localTransform());
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

	void CameraNode::set_position(const glm::vec3& position)
	{
		Node::set_position(position);
	}

	// // ----- GETTERS ----- // //
	glm::mat4 CameraNode::worldToCam_matrix()
	{
		return  m_camera->object_to_cam() * Node::worldTransform();
	}

	glm::mat4 CameraNode::worldToProjection_matrix()
	{
		return m_camera->transform_to_projection(worldTransform());
	}
} // namespace gl_engine