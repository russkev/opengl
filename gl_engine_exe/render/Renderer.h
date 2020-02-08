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
#include "../material/Material.h"
#include "../mesh/WindowQuad.h"
#include "../mesh/Cube.h"
#include "../mesh/Plane.h"
#include "../post/ToneMap.h"
#include "../post/Bloom.h"
#include "../post/PostEffect.h"
#include "DeferredRender.h"

namespace gl_engine_exe
{
	// // ----- FORWARD DECLERATIONS ----- // //
	struct CameraNode;
	struct LightNode;
	struct Node;
	struct LightNode;
	struct ShadowMap;
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
	private:
		void clear_screen();
		void render_shadow_maps();
		void render_geometry();
		void render_lights();

		// // ----- GENERAL METHODS ----- // //
	public:
		bool poll_events();
		void update(Window * window, Timer* timer);
	private:
		void init_settings();
		void init_first_frame();
		void init_post_effects();
		void init_deferred_renderer();

		// // ----- SETTERS ----- // //
	public:
		void add_node(Node* node);
		void enable_post_effects();
		void disable_post_effects();
		void enable_deferred_render();
		void disable_deferred_render();

	private:
		void add_light_nodes(Node* root_node);
		void add_material(Material* material);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		CameraNode* m_cameraNode;
		std::vector<LightNode*> m_light_nodes;
		std::vector<ShadowMap*> m_shadow_maps;
		std::vector<Material*> m_materials;
		std::map<std::string, Node*> m_root_nodes;
		glm::uvec2 m_dimensions;
		bool m_first_frame = true;
		bool m_post_effects_enabled = true;
		bool m_deferred_render_enabled = false;

		// // ----- BACKBUFFER VARIABLES ----- // //
		Framebuffer m_backbuffer_FBO{ GL_FRAMEBUFFER };

		Texture m_backbuffer_depth{ Texture::create_depth_null_texture(GL_TEXTURE_2D, &m_dimensions) };

		ToneMap m_tone_map{ &m_backbuffer_FBO, &m_dimensions };
		Bloom m_bloom{ &m_backbuffer_FBO, &m_dimensions, &m_tone_map };

		// // ----- DEFERRRED RENDER VARIABLES ----- // //
		DeferredRender m_deferred_render;
		//Framebuffer m_g_buffer_FBO{ GL_FRAMEBUFFER };

		//Texture m_g_position;
		//Texture m_g_normal;
		//Texture m_g_color_spec;
		//Texture m_g_depth;

		//BlinnDeferredMaterial m_deferred_material;
		//
		//Mesh m_deferred_mesh{ WindowQuad::create_windowQuad() };
		//MeshNode m_deferred_mesh_node{ "Deferred Screen Node", &m_deferred_mesh, &m_deferred_material };

	};
} // namespace gl_engine
#endif