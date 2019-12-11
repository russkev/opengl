#ifndef GL_ENGINE_RENDER_RENDERER_H
#define GL_ENGINE_RENDER_RENDERER_H

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>


// Forward declerations
struct CameraNode;
struct LightNode;
struct Node;
struct LightNode;
struct Material;
struct Window;
struct Timer;

/*

	Renderer object for rendering all objects

*/
struct Renderer
{
private:
	// // ----- MEMBER VARIABLES ----- // //
	CameraNode* m_cameraNode;
	//LightNode* m_lightNode;
	std::vector<LightNode*> m_lightNodes;
	std::vector<Material*> m_materials;
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
	void addLightNode(LightNode* lightNode);
	void addNode(Node* node);
	bool pollEvents();
	void update(Window * window, Timer* timer);

private:
	void addMaterial(Material* material);


	// // ----- GETTERS ----- // //
public:
	const CameraNode* camera() const;

	// // ----- SETTERS ----- // //
	CameraNode* camera();



};

#endif