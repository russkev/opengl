#ifndef GL_ENGINE_RENDER_RENDERER_H
#define GL_ENGINE_RENDER_RENDERER_H

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "../node/CameraNode.h"

namespace gl_engine
{
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
		std::vector<LightNode*> m_lightNodes;
		std::vector<Material*> m_materials;
		std::map<std::string, Node*> m_root_nodes;
		glm::uvec2 m_dimensions = { 800, 600 };
		bool m_first_frame = true;

	public:
		// // ----- CONSTRUCTOR ----- // //
		Renderer(CameraNode* camera);
		Renderer(CameraNode* camera, const glm::uvec2& dimensions);

		// // ----- RENDER ----- // //
		void render();

		// // ----- GENERAL METHODS ----- // //
		void init_settings();
		void init_first_frame();
		void add_light_node(LightNode* lightNode);
		void add_node(Node* node);
		bool poll_events();
		void update(Window * window, Timer* timer);

	private:
		void add_material(Material* material);

	};
} // namespace gl_engine
#endif