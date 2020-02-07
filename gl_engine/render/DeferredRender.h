#ifndef GL_ENGINE_RENDER_DEFERRED_RENDER_H
#define GL_ENGINE_RENDER_DEFERRED_RENDER_H

#include <glm/glm.hpp>

#include "../shading/Framebuffer.h"
#include "../shading/Texture.h"
#include "../mesh/Mesh.h"
#include "../mesh/WindowQuad.h"
#include "../material/MaterialLibrary.h"
#include "../node/MeshNode.h"

namespace gl_engine
{
	// // ----- FORWARD DECLERATION ----- // //
	struct CameraNode;

	/*
	
	Struct for adding a deferred render stage to the primary renderer

	*/
	struct DeferredRender
	{
		// // ----- CONSTRUCTOR ----- // //
		DeferredRender(const GLenum target, const glm::uvec2& dimensions);

		// // ----- GENERAL ----- // //
		void bind();
		void unbind();
		void update_view(const CameraNode* camera_node);
		void draw();

		// // ----- GETTERS ----- // //
		glm::uvec2 dimensions();
		Framebuffer* framebuffer();
		Material* material();
		Mesh* mesh();
		MeshNode* mesh_node();


		// // ----- MEMBER VARIABLES ----- // //
		glm::uvec2 m_dimensions;

		Framebuffer m_g_buffer_FBO{ GL_FRAMEBUFFER };

		Texture m_g_position;
		Texture m_g_normal;
		Texture m_g_color_spec;
		Texture m_g_depth;

		BlinnDeferredMaterial m_deferred_material;

		Mesh m_deferred_mesh{ WindowQuad::create_windowQuad() };
		MeshNode m_deferred_mesh_node{ "Deferred Screen Node", &m_deferred_mesh, &m_deferred_material };
	};



} // namespace gl_engine

#endif