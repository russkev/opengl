#ifndef GL_ENGINE_NODE_CAMERA_H
#define GL_ENGINE_NODE_CAMERA_H

#include <glm/glm.hpp>

#include "Node.h"
#include "../camera/Camera.h"
#include "../Buffer.h"

/*

	Main mesh type node. Links a mesh to a material so it can be rendered on screen

*/

struct CameraNode : public Node, public Camera
{
	// // ----- MEMBER VARIABLES ----- // //
private:
	//std::string m_name;
	//Camera* m_camera;

	// // ----- CONSTRUCTOR ----- // //
public:
	CameraNode() {};
	CameraNode(const std::string name);

	// // ----- SETTERS ----- // //
	//Camera* camera();

	// // ----- GETTERS ----- // //
	//const std::string& name() const;
	//const Camera* camera() const;

	// // ----- OVERRIDES ----- // //
	const glm::vec3 position() const override;
	const glm::vec3 worldPosition() override;
	void setPosition(const glm::vec3&) override;
	glm::mat4 worldToCam_matrix() override;
	glm::mat4 worldToProjection_matrix() override;

};



#endif