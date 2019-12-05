#ifndef GL_ENGINE_RENDER_RENDERER_H
#define GL_ENGINE_RENDER_RENDERER_H

#include <map>

#include <GL/glew.h>
#include "../Window.h"

#include "../node/Node.h"
#include "../node/CameraNode.h"


/*

	Renderer object for rendering all objects

*/
struct Renderer
{
private:
	// // ----- MEMBER VARIABLES ----- // //
	CameraNode* m_cameraNode;
	std::map<std::string, Node*> m_root_nodes;
	glm::uvec2 m_dimensions = { 800, 600 };

public:
	// // ----- CONSTRUCTOR ----- // //
	Renderer(CameraNode* camera);
	Renderer(CameraNode* camera, const glm::uvec2& dimensions);

	// // ----- RENDER ----- // //
	void render();

	// // ----- GENERAL METHODS ----- // //
	void initSettings();
	void addNode(Node* node);
	bool pollEvents();

	void go(Window*);

	// // ----- GETTERS ----- // //
	const CameraNode* camera() const;

	// // ----- SETTERS ----- // //
	CameraNode* camera();



};

#endif