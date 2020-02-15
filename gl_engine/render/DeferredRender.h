#ifndef GL_ENGINE_RENDER_DEFERRED_RENDER_H
#define GL_ENGINE_RENDER_DEFERRED_RENDER_H

#include <unordered_set>
#include <unordered_map>

#include <glm/glm.hpp>

#include "shading/Framebuffer.h"
#include "shading/Texture.h"
#include "mesh/Mesh.h"
#include "mesh/WindowQuad.h"
#include "material/MaterialLibrary.h"
#include "node/MeshNode.h"

namespace glen
{
	// // ----- FORWARD DECLERATION ----- // //
	struct CameraNode;

	/*
	
	Struct for adding a deferred render stage to the primary renderer

	*/
	struct DeferredRender
	{
		// // ----- CONSTRUCTOR ----- // //
		DeferredRender() {};
		DeferredRender(const GLenum target, const glm::uvec2& dimensions);
		DeferredRender(const DeferredRender& other) = delete;
		DeferredRender(DeferredRender&& other);
		DeferredRender& operator = (const DeferredRender& other) = delete;
		DeferredRender& operator = (DeferredRender&& other);
		~DeferredRender() = default;

		// // ----- GENERAL ----- // //
		void bind();
		void unbind();
		void update_view(const CameraNode* camera_node);
		void draw();

		// // ----- FACTORIES ----- // //
		void setup_blinn_deferred(const GLenum target, const glm::uvec2& dimensions);
		//static DeferredRender create_blinn_deferred(const GLenum target, const glm::uvec2& dimensions);
		//static DeferredRender create_ao_g_buffer(const GLenum target, const glm::uvec2& dimensions);

		// // ----- GETTERS ----- // //
		glm::uvec2 dimensions();
		Framebuffer* framebuffer();
		Material* material();
		Mesh* mesh();
		MeshNode* mesh_node();
		const Texture* texture(const std::string& name);
		const Texture* depth_texture();

		// // ----- SETTERS ----- // //
		void set_target(const GLenum target);
		void set_dimensions(const glm::uvec2& dimensions);
		void set_color_texture(const std::string& name, Texture* texture);
		void set_depth_texture(Texture* texture);
		void send_color_textures_to_framebuffer();
	private:
		void set_color_texture(const std::string& name, Texture texture);
		void set_depth_texture(Texture texture);

		// // ----- MEMBER VARIABLES ----- // //
		GLenum m_target;
		glm::uvec2 m_dimensions;

		Framebuffer m_g_buffer_FBO{ GL_FRAMEBUFFER };

		//Texture m_g_position;
		//Texture m_g_normal;
		//Texture m_g_color_spec;
		Texture m_g_depth;

		std::unordered_map<std::string, Texture> m_textures;

		BlinnDeferredMaterial m_deferred_material;

		Mesh m_deferred_mesh{ WindowQuad::create_windowQuad() };
		MeshNode m_deferred_mesh_node{ "Deferred Screen Node", &m_deferred_mesh, &m_deferred_material };
	};
}
#endif