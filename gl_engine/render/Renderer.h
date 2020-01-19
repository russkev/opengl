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
#include "../post/ToneMap.h"
#include "../post/DepthMap.h"
#include "../post/Bloom.h"

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
	private:
		void clear_screen();
		void render_shadow_maps();
		void render_geometry();

		// // ----- GENERAL METHODS ----- // //
	public:
		bool poll_events();
		void update(Window * window, Timer* timer);
	private:
		void init_settings();
		void init_first_frame();

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
		glm::uvec2 m_dimensions;
		bool m_first_frame = true;

		// // ----- BACKBUFFER VARIABLES ----- // //
		Framebuffer m_backbuffer_FBO{ GL_FRAMEBUFFER };

		ToneMap m_tone_map{ &m_backbuffer_FBO, &m_dimensions };
		DepthMap m_depth_map{ &m_backbuffer_FBO, &m_dimensions };
		Bloom m_bloom{ &m_backbuffer_FBO, &m_dimensions, &m_tone_map };
	};
} // namespace gl_engine
#endif