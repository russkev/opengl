#ifndef GL_ENGINE_NODE_CAMERA_H
#define GL_ENGINE_NODE_CAMERA_H

#include <glm/glm.hpp>

#include "Node.h"
#include "../Buffer.h"

// // ----- FORWARD DECLERATION ----- // //
struct Camera;

/*

	Main mesh type node. Links a mesh to a material so it can be rendered on screen

*/
struct CameraNode : public Node
{
	// // ----- MEMBER VARIABLES ----- // //
private:
	Camera* m_camera;

	// // ----- CONSTRUCTOR ----- // //
public:
	CameraNode() {};
	CameraNode(const std::string name, Camera* camera);

	// // ----- GETTERS ----- // //
	Camera* camera();
	const glm::vec3 position() const override;
	const glm::vec3 worldPosition() override;
	glm::mat4 worldToCam_matrix();
	glm::mat4 worldToProjection_matrix();

	// // ----- SETTERS ----- // //
	void setPosition(const glm::vec3&) override;
};



#endif