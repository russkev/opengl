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
		//glm::mat4& localTransform() override;
		const glm::vec3 position() const override;
		const glm::vec3 worldPosition() override;

		glm::mat4 objectToCam_matrix();
		glm::mat4 worldToProjection_matrix();

		// // ----- SETTERS ----- // //
		void setPosition(const glm::vec3&) override;
	};
} // namespace gl_engine
#endif