#ifndef GL_ENGINE_NODE_CAMERA_H
#define GL_ENGINE_NODE_CAMERA_H

#include <glm/glm.hpp>

#include "Node.h"
#include "../Buffer.h"

namespace gl_engine
{
	// // ----- FORWARD DECLERATION ----- // //
	struct Camera;

	/*

		Camera node. Inherits from node.

	*/
	struct CameraNode : public Node
	{
		// // ----- MEMBER VARIABLES ----- // //
	private:
		Camera* m_camera;

		// // ----- CONSTRUCTORS ----- // //
	public:
		CameraNode() {};
		CameraNode(const std::string name, Camera* camera);

		void update();

		// // ----- GETTERS ----- // //
		Camera* camera();
		const glm::vec3 world_position() override;

		glm::mat4 world_to_cam();
		glm::mat4 world_to_projection();

		// // ----- SETTERS ----- // //
		void set_position(const glm::vec3&) override;
	};
} // namespace gl_engine
#endif