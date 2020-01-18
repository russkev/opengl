#ifndef GL_ENGINE_RENDER_RENDERER_H
#define GL_ENGINE_RENDER_RENDERER_H

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "../node/CameraNode.h"
#include "../node/MeshNode.h"
#include "../shading/Framebuffer.h"
#include "../shading/Texture.h"
#include "../shading/Material.h"
#include "../mesh/WindowQuad.h"
#include "../mesh/Cube.h"
#include "../mesh/Plane.h"

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
		bool poll_events();
		void update(Window * window, Timer* timer);
	private:
		void init_settings();
		void init_first_frame();
		void init_hdr();
		void init_backbuffer_color(Texture& backbuffer);
		void init_backbuffer_depth(Texture& backbuffer);
		void init_color_attachments();

		// // ----- SETTERS ----- // //
	public:
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

		// // ----- BACKBUFFER VARIABLES ----- // //
		Framebuffer m_backbuffer_FBO{ GL_FRAMEBUFFER };
		Framebuffer m_backbuffer_pingpong_FBO{ GL_FRAMEBUFFER };
		Texture m_backbuffer_colorA{ GL_TEXTURE_2D };
		Texture m_backbuffer_colorB{ GL_TEXTURE_2D };
		Texture m_backbuffer_pingpong{ GL_TEXTURE_2D };
		Texture m_backbuffer_depth{ GL_TEXTURE_2D };

		Material m_hdr_material{ "HDR Shader", "screenPassthrough.vert", "HDR.frag" };
		Material m_bloom_material{ "Bloom Shader", "screenPassthrough.vert", "Bloom.frag" };
		Mesh m_screen_mesh{ WindowQuad::create_windowQuad() };
		MeshNode m_hdr_screen_node{ "HDR Screen Node", &m_screen_mesh, &m_hdr_material };
		MeshNode m_bloom_screen_node{ "Bloom Screen Node", &m_screen_mesh, &m_bloom_material };

	};
} // namespace gl_engine
#endif