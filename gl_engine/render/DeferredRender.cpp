#include "pch.h"
#include "DeferredRender.h"

namespace glen
{
	// // ----- CONSTRUCTOR ----- // //
	DeferredRender::DeferredRender(const GLenum target, const glm::uvec2& dimensions) :
		m_target{ target },
		m_dimensions{ dimensions }
	{}

	// // ----- GENERAL ----- // //
	void DeferredRender::bind()
	{
		m_g_buffer_FBO.bind();
	}

	void DeferredRender::unbind()
	{
		m_g_buffer_FBO.unbind();
	}

	void DeferredRender::update_view(const CameraNode* camera_node)
	{
		m_deferred_material.update_view(camera_node, NULL);
	}

	void DeferredRender::draw()
	{
		m_deferred_mesh_node.draw();
	}

	// // ----- FACTORIES ----- // //
	DeferredRender DeferredRender::create_blinn_deferred(const GLenum target, const glm::uvec2& dimensions)
	{
		DeferredRender deferred_render(target, dimensions);
		Texture g_position{		Texture::create_16bit_rgb_null_texture(target, dimensions) };
		Texture g_normal{		Texture::create_16bit_rgb_null_texture(target, dimensions) };
		Texture g_color_spec{	Texture::create_8bit_rgba_null_texture(target, dimensions) };
		Texture g_depth{		Texture::create_depth_null_texture(target, dimensions) };

		deferred_render.framebuffer()->push_back_color_buffer_textures( std::vector<const Texture*>{
			&g_position, &g_normal, &g_color_spec	});
		deferred_render.framebuffer()->set_depth_buffer_texture(&g_depth);

		deferred_render.material()->set_texture(BlinnDeferredMaterial::k_g_position, &g_position);
		deferred_render.material()->set_texture(BlinnDeferredMaterial::k_g_normal, &g_normal);
		deferred_render.material()->set_texture(BlinnDeferredMaterial::k_g_diffuse_spec, &g_color_spec);

		return deferred_render;
	}

	// // ----- GETTERS ----- // //
	glm::uvec2 DeferredRender::dimensions() 
	{
		return m_dimensions;
	}

	Framebuffer* DeferredRender::framebuffer()
	{
		return &m_g_buffer_FBO;
	}

	Material* DeferredRender::material()
	{
		return &m_deferred_material;
	}

	Mesh* DeferredRender::mesh()
	{
		return &m_deferred_mesh;
	}

	MeshNode* DeferredRender::mesh_node()
	{
		return &m_deferred_mesh_node;
	}

	// // ----- SETTERS ----- // //
	void DeferredRender::set_target(const GLenum target)
	{
		m_target = target;
	}

	void DeferredRender::set_dimensions(const glm::uvec2& dimensions)
	{
		m_dimensions = dimensions;
	}
}