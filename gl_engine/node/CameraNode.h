#ifndef GL_ENGINE_NODE_CAMERA_H
#define GL_ENGINE_NODE_CAMERA_H

#include "Node.h"
#include "../camera/Camera.h"
#include "../Buffer.h"

/*

	Main mesh type node. Links a mesh to a material so it can be rendered on screen

*/

struct CameraNode : public Node
{
	// // ----- MEMBER VARIABLES ----- // //
private:
	std::string m_name;
	Camera* m_camera;

	// // ----- CONSTRUCTOR ----- // //
public:
	CameraNode() {};
	CameraNode(const std::string name, Camera* camera);

	// // ----- SETTERS ----- // //
	Camera* camera();

	// // ----- GETTERS ----- // //
	const std::string& name() const;
	const Camera* camera() const;


};



#endif