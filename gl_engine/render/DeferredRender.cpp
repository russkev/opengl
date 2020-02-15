#include "pch.h"
#include "DeferredRender.h"

namespace glen
{
	// // ----- CONSTRUCTOR ----- // //
	DeferredRender::DeferredRender(const GLenum target, const glm::uvec2& dimensions) :
		m_target{ target },
		m_dimensions{ dimensions }
	{}

	DeferredRender::DeferredRender(DeferredRender&& other) :
		m_target{ other.m_target },
		m_dimensions{ std::exchange(other.m_dimensions, glm::uvec2{ 0u }) },
		//m_external_textures{ std::move(other.m_external_textures)},
		m_deferred_material{ std::move(other.m_deferred_material)},
		m_deferred_mesh{ std::move(other.m_deferred_mesh) },
		m_deferred_mesh_node{ other.m_deferred_mesh_node.name(), &m_deferred_mesh, &m_deferred_material }
	{
		bool using_local_depth_texture = other.m_g_buffer_FBO.depth_texture() == &other.m_g_depth;

		relink_framebuffer_color_textures(other.m_g_buffer_FBO.color_textures());

		m_g_buffer_FBO = std::move(other.m_g_buffer_FBO);
		m_g_depth = std::move(other.m_g_depth);
		m_internal_textures = std::move(other.m_internal_textures);
		if (using_local_depth_texture)
		{
			m_g_buffer_FBO.set_depth_buffer_texture(&m_g_depth);
		}

	}

	void DeferredRender::relink_framebuffer_color_textures(const std::vector<const Texture*>& framebuffer_textures)
	{
		for (const Texture* framebuffer_texture : framebuffer_textures)
		{
			for (const auto & texture_pair : m_internal_textures)
			{
				if (&texture_pair.second == framebuffer_texture)
				{
					framebuffer_texture = &m_internal_textures[texture_pair.first];
				}
			}
		}
	}



	DeferredRender& DeferredRender::operator = (DeferredRender&& other)
	{
		(*this).~DeferredRender();
		return *new (this) DeferredRender(std::move(other));
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

	// // ----- FACTORIES ----- // //	
	DeferredRender DeferredRender::create_blinn_deferred(const GLenum target, const glm::uvec2& dimensions)
	{
		DeferredRender deferred_render{ target, dimensions };

		deferred_render.set_color_texture(BlinnDeferredMaterial::k_g_position, Texture::create_16bit_rgb_null_texture(target, dimensions));
		deferred_render.set_color_texture(BlinnDeferredMaterial::k_g_normal, Texture::create_16bit_rgb_null_texture(target, dimensions));
		deferred_render.set_color_texture(BlinnDeferredMaterial::k_g_diffuse_spec, Texture::create_8bit_rgba_null_texture(target, dimensions));
		deferred_render.set_depth_texture(Texture::create_depth_null_texture(target, dimensions));
		
		deferred_render.send_color_textures_to_framebuffer();

		return deferred_render;
	}

	//DeferredRender DeferredRender::create_ao_g_buffer(const GLenum target, const glm::uvec2& dimensions)
	//{
	//	DeferredRender deferred_render{ target, dimensions };

	//	deferred_render.set_color_texture(AO_GBufferMaterial::k_g_position, Texture::create_16bit_rgb_null_texture(target, dimensions));
	//	deferred_render.set_color_texture(AO_GBufferMaterial::k_g_normal, Texture::create_16bit_rgb_null_texture(target, dimensions));
	//	deferred_render.set_color_texture(AO_GBufferMaterial::k_g_diffuse, Texture::create_8bit_rgb_null_texture(target, dimensions));
	//	deferred_render.set_depth_texture(Texture::create_depth_null_texture(target, dimensions));

	//	deferred_render.send_color_textures_to_framebuffer();
	//	return deferred_render;
	//}

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

	const Texture* DeferredRender::texture(const std::string& name)
	{
		return &m_internal_textures[name];
	}

	const Texture* DeferredRender::depth_texture()
	{
		return &m_g_depth;
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

	void DeferredRender::set_color_texture(const std::string& name, Texture texture)
	{
		m_internal_textures[name] = std::move(texture);
		m_deferred_material.set_texture(name, &m_internal_textures[name]);

		//set_color_texture(name, &m_textures[name]);

	}

	void DeferredRender::set_color_texture(const std::string& name, Texture* texture)
	{
		m_external_textures[name] = texture;
		m_deferred_material.set_texture(name, texture);
	}

	void DeferredRender::set_depth_texture(Texture texture)
	{
		m_g_depth = std::move(texture);
		set_depth_texture(&m_g_depth);
	}

	void DeferredRender::set_depth_texture(Texture* texture)
	{
		m_g_buffer_FBO.set_depth_buffer_texture(texture);
	}

	void DeferredRender::send_color_textures_to_framebuffer()
	{
		std::vector<const Texture*> texture_vector;
		for (const auto & texture_pair : m_internal_textures)
		{
			texture_vector.push_back(&texture_pair.second);
		}
		for (const auto & texture_pair : m_external_textures)
		{
			texture_vector.push_back(texture_pair.second);
		}
		m_g_buffer_FBO.push_back_color_buffer_textures(texture_vector);
	}
}