#ifndef GL_ENGINE_RENDER_RENDERER_H
#define GL_ENGINE_RENDER_RENDERER_H

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "node/CameraNode.h"
#include "node/MeshNode.h"
#include "shading/Framebuffer.h"
#include "shading/Texture.h"
#include "material/Material.h"
#include "mesh/WindowQuad.h"
#include "mesh/Cube.h"
#include "mesh/Plane.h"
#include "post/ToneMap.h"
#include "post/Bloom.h"
#include "post/PostEffect.h"
#include "post/Deferred.h"


int main(int argc, char* args[]);

namespace glen
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
		void render_geometry(Material* material);
		void render_lights();

		// // ----- GENERAL METHODS ----- // //
	public:
		bool poll_events();
		void update(Window * window, Timer* timer);
	private:
		void init_settings();
		void init_first_frame();
		void init_post_effects();
		void init_post_beauty();
		void init_deferred_renderer();
		void init_ao();

		// // ----- SETTERS ----- // //
	public:
		void add_node(Node* node);
		void enable_post_effects();
		void disable_post_effects();
		void enable_deferred_render();
		void disable_deferred_render();
		void enable_ao();
		void disable_ao();

	private:
		void add_light_nodes(Node* root_node);
		void add_material(Material* material);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		CameraNode* m_camera_node;
		std::vector<LightNode*> m_light_nodes;
		std::vector<ShadowMap*> m_shadow_maps;
		std::vector<Material*> m_materials;
		std::map<std::string, Node*> m_root_nodes;
		glm::uvec2 m_dimensions;
		bool m_first_frame = true;
		bool m_post_effects_enabled = true;
		bool m_deferred_render_enabled = false;
		bool m_ao_enabled = false;

		// // ----- BACKBUFFER VARIABLES ----- // //
		Framebuffer m_backbuffer_FBO{ GL_FRAMEBUFFER };
		Framebuffer m_ao_backbuffer_FBO{ GL_FRAMEBUFFER };

		Texture m_backbuffer_depth{ Texture::create_depth_null_texture(GL_TEXTURE_2D, m_dimensions) };

		ToneMap m_tone_map{ &m_backbuffer_FBO, m_dimensions };
		Bloom m_bloom{ m_dimensions, &m_tone_map };

		// // ----- BEAUTY ----- // //
		Framebuffer m_beauty_FBO{ GL_FRAMEBUFFER };
		Texture m_beauty_texture{ Texture::create_16bit_rgba_null_texture(GL_TEXTURE_2D, m_dimensions) };

		// // ----- DEFERRRED RENDER VARIABLES ----- // //
		Framebuffer m_g_buffer{ GL_FRAMEBUFFER };
		BlinnDeferred m_blinn_deferred;

		// // ----- AO VARIABLES ----- // //
		AO_GBufferMaterial m_ao_g_buffer_material{};
		Framebuffer m_ao_g_buffer_FBO{ GL_FRAMEBUFFER };
		AO_GBufferDeferred m_ao_g_buffer_deferred;

		Framebuffer m_ao_FBO{ GL_FRAMEBUFFER };
		AO_BlurDeferred m_ao_blur_deferred;


	};
}
#endif