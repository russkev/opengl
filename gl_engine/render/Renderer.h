#ifndef GL_ENGINE_RENDER_RENDERER_H
#define GL_ENGINE_RENDER_RENDERER_H

#include <map>

#include <GL/glew.h>

#include "../Timer.h"
#include "../Window.h"
#include "../node/Node.h"
#include "../node/CameraNode.h"
#include "../node/LightNode.h"


#include "../light/PointLight.h"
#include "../shading/Material.h"


/*

	Renderer object for rendering all objects

*/
struct Renderer
{
private:
	// // ----- MEMBER VARIABLES ----- // //
	CameraNode* m_cameraNode;
	LightNode* m_lightNode;
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
	void addLightNode(LightNode* lightNode);
	bool pollEvents();
	void update(Window * window, Timer* timer);

	// // ----- GETTERS ----- // //
	const CameraNode* camera() const;

	// // ----- SETTERS ----- // //
	CameraNode* camera();



};

#endif