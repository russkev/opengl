#include "CameraNode.h"

#include "../camera/Camera.h"
#include "../utils/VectorUtils.h"



namespace gl_engine
{
	// // ----- CONSTRUCTORS ----- // //
	CameraNode::CameraNode(const std::string name, Camera* camera) :
		Node(name), m_camera(camera)
	{
		m_camera->register_transform(&Node::local_to_node());
	}

	// // ----- GENERAL METHODS ----- // //
	void CameraNode::update()
	{
		m_camera->update();
	}

	// // ----- GETTERS ----- // //
	Camera* CameraNode::camera()
	{
		return m_camera;
	}

	const glm::vec3 CameraNode::world_position()
	{
		return glm::vec3(world_to_node() * glm::vec4(m_camera->position(), 1.0f));
	}

	glm::mat4 CameraNode::world_to_cam()
	{
		return  m_camera->object_to_cam() * Node::world_to_node();
	}

	glm::mat4 CameraNode::world_to_projection()
	{
		return m_camera->transform_to_projection(world_to_node());
	}

	// // ----- SETTERS ----- // //
	void CameraNode::set_position(const glm::vec3& position)
	{
		Node::set_position(position);
	}

} // namespace gl_engine