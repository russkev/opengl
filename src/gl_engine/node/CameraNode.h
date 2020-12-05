#ifndef GL_ENGINE_NODE_CAMERA_H
#define GL_ENGINE_NODE_CAMERA_H

#include "Node.h"
#include "resources/Buffer.h"

namespace glen
{
	// // ----- FORWARD DECLERATION ----- // //
	struct Camera;

	/*

		Camera node. Inherits from node.

	*/
	struct CameraNode : public Node
	{
		// // ----- CONSTRUCTORS ----- // //
	public:
		CameraNode(const std::string name, Camera* camera);

		// // ----- GENERAL METHODS ----- // //
		void update();

		// // ----- GETTERS ----- // //
		const Camera* camera() const;
		const glm::vec3 world_position() const override;

		const glm::mat4 world_to_cam() const;
		const glm::mat4 world_to_projection() const;

		// // ----- SETTERS ----- // //
		void set_position(const glm::vec3&) override;
		Camera* camera();

		// // ----- MEMBER VARIABLES ----- // //
	private:
		Camera* m_camera{ NULL };
	};
}
#endif