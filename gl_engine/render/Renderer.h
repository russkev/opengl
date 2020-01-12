#ifndef GL_ENGINE_RENDER_RENDERER_H
#define GL_ENGINE_RENDER_RENDERER_H

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "../node/CameraNode.h"
#include "../shading/Framebuffer.h"
#include "../shading/Texture.h"
#include "../shading/Material.h"

namespace gl_engine
{
	// // ----- FORWARD DECLERATIONS ----- // //
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
	public:
		// // ----- CONSTRUCTOR ----- // //
		Renderer(CameraNode* camera, const glm::uvec2& dimensions);

		// // ----- RENDER ----- // //
		void render();

		// // ----- GENERAL METHODS ----- // //
		void init_settings();
		void init_first_frame();
		void init_hdr();
		bool poll_events();
		void update(Window * window, Timer* timer);

		// // ----- SETTERS ----- // //
		void add_node(Node* node);

	private:
		void add_light_nodes(Node* root_node);
		void add_material(Material* material);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		CameraNode* m_cameraNode;
		std::vector<LightNode*> m_lightNodes;
		std::vector<Material*> m_materials;
		std::map<std::string, Node*> m_root_nodes;
		glm::uvec2 m_dimensions = { 800, 600 };
		bool m_first_frame = true;

		Framebuffer m_hdr_framebuffer{ GL_FRAMEBUFFER };
		Texture m_hdr_texture{ GL_TEXTURE_2D };
		GLuint m_rbo_depth_id;
		Material m_hdr_material{ "HDR Shader", "HDR.vert", "HDR.frag" };
	};
} // namespace gl_engine
#endif