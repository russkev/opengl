#include "DeferredRender.h"

namespace gl_engine_exe
{
	// // ----- CONSTRUCTOR ----- // //
	DeferredRender::DeferredRender(const GLenum target, const glm::uvec2& dimensions) :
		m_dimensions{ dimensions },
		m_g_position{ Texture::create_16bit_rgb_null_texture(target, &m_dimensions) },
		m_g_normal{ Texture::create_16bit_rgb_null_texture(target, &m_dimensions) },
		m_g_color_spec{ Texture::create_8bit_rgba_null_texture(target, &m_dimensions) },
		m_g_depth{ Texture::create_depth_null_texture(target, &m_dimensions) }
	{
		m_g_buffer_FBO.push_back_color_buffer_textures(std::vector<const Texture*>{
			&m_g_position, &m_g_normal, &m_g_color_spec	});

		m_g_buffer_FBO.set_depth_buffer_texture(&m_g_depth);

		m_deferred_material.set_texture(BlinnDeferredMaterial::k_g_position, &m_g_position);
		m_deferred_material.set_texture(BlinnDeferredMaterial::k_g_normal, &m_g_normal);
		m_deferred_material.set_texture(BlinnDeferredMaterial::k_g_diffuse_spec, &m_g_color_spec);
	}

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
} // namespace gl_engine