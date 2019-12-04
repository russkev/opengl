#pragma once

#include <map>

#include <GL/glew.h>

#include "../node/Node.h"
#include "../node/CameraNode.h"

/*

	Renderer object for rendering all objects

*/
struct Renderer
{
	// // ----- MEMBER VARIABLES ----- // //
	CameraNode* m_camera_node;
	std::map<std::string, Node*> m_root_nodes;
	glm::ivec2 m_dimensions = { 1024, 576 };

	// // ----- CONSTRUCTOR ----- // //
	Renderer(CameraNode* camera);
	Renderer(CameraNode* camera, const glm::ivec2& dimensions);

	// // ----- RENDER ----- // //
	void render();

	// // ----- GENERAL METHODS ----- // //
	void addNode(Node* node);

	// // ----- GETTERS ----- // //
	const CameraNode* camera() const;

	// // ----- SETTERS ----- // //
	CameraNode* camera();



};