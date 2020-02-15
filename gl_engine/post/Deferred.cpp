#include "pch.h"
#include "Deferred.h"

namespace glen
{
	// // ----- CONSTRUCTOR ----- // //
	Deferred::Deferred(const GLenum target, Framebuffer* g_buffer, const glm::uvec2& dimensions) :
		m_target{ target },
		m_dimensions{ dimensions },
		m_g_buffer_FBO{ g_buffer }
	{}

	Deferred::Deferred(Deferred&& other) :
		m_target{ other.m_target },
		m_dimensions{ std::exchange(other.m_dimensions, glm::uvec2{ 0u }) },
		m_external_textures{ std::move(other.m_external_textures)},
		m_deferred_material{ std::move(other.m_deferred_material)},
		m_deferred_mesh_node{ other.m_deferred_mesh_node.name(), PostEffect::mesh(), &m_deferred_material }
	{
		bool using_local_depth_texture = other.m_g_buffer_FBO->depth_texture() == &other.m_g_depth;

		relink_framebuffer_color_textures(other.m_g_buffer_FBO->color_textures());

		std::swap(m_g_buffer_FBO, other.m_g_buffer_FBO);
		m_g_depth = std::move(other.m_g_depth);
		m_internal_textures = std::move(other.m_internal_textures);
		if (using_local_depth_texture)
		{
			m_g_buffer_FBO->set_depth_buffer_texture(&m_g_depth);
		}

	}

	void Deferred::relink_framebuffer_color_textures(const std::vector<const Texture*>& framebuffer_textures)
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



	Deferred& Deferred::operator = (Deferred&& other)
	{
		(*this).~Deferred();
		return *new (this) Deferred(std::move(other));
	}


	// // ----- GENERAL ----- // //
	void Deferred::bind()
	{
		m_g_buffer_FBO->bind();
	}

	void Deferred::unbind()
	{
		m_g_buffer_FBO->unbind();
	}

	void Deferred::update_view(const CameraNode* camera_node)
	{
		m_deferred_material.update_view(camera_node, NULL);
	}

	void Deferred::draw()
	{
		m_deferred_mesh_node.draw();
	}

	// // ----- FACTORIES ----- // //	
	Deferred Deferred::create_blinn_deferred(const GLenum target, Framebuffer* g_buffer, const glm::uvec2& dimensions)
	{
		Deferred deferred_render{ target, g_buffer, dimensions };

		deferred_render.set_color_texture(BlinnDeferredMaterial::k_g_position, Texture::create_16bit_rgb_null_texture(target, dimensions));
		deferred_render.set_color_texture(BlinnDeferredMaterial::k_g_normal, Texture::create_16bit_rgb_null_texture(target, dimensions));
		deferred_render.set_color_texture(BlinnDeferredMaterial::k_g_diffuse_spec, Texture::create_8bit_rgba_null_texture(target, dimensions));
		deferred_render.set_depth_texture(Texture::create_depth_null_texture(target, dimensions));
		
		deferred_render.send_color_textures_to_framebuffer();

		return deferred_render;
	}

	//Deferred Deferred::create_ao_g_buffer(const GLenum target, Framebuffer* g_buffer, const glm::uvec2& dimensions)
	//{
	//	Deferred deferred_render{ target, dimensions };
	//	m_g_buffer = g_buffer;

	//	deferred_render.set_color_texture(AO_GBufferMaterial::k_g_position, Texture::create_16bit_rgb_null_texture(target, dimensions));
	//	deferred_render.set_color_texture(AO_GBufferMaterial::k_g_normal, Texture::create_16bit_rgb_null_texture(target, dimensions));
	//	deferred_render.set_color_texture(AO_GBufferMaterial::k_g_diffuse, Texture::create_8bit_rgb_null_texture(target, dimensions));
	//	deferred_render.set_depth_texture(Texture::create_depth_null_texture(target, dimensions));

	//	deferred_render.send_color_textures_to_framebuffer();
	//	return deferred_render;
	//}

	// // ----- GETTERS ----- // //
	glm::uvec2 Deferred::dimensions() 
	{
		return m_dimensions;
	}

	Framebuffer* Deferred::framebuffer()
	{
		return m_g_buffer_FBO;
	}

	Material* Deferred::material()
	{
		return &m_deferred_material;
	}

	MeshNode* Deferred::mesh_node()
	{
		return &m_deferred_mesh_node;
	}

	const Texture* Deferred::texture(const std::string& name)
	{
		return &m_internal_textures[name];
	}

	const Texture* Deferred::depth_texture()
	{
		return &m_g_depth;
	}

	// // ----- SETTERS ----- // //
	void Deferred::set_target(const GLenum target)
	{
		m_target = target;
	}

	void Deferred::set_dimensions(const glm::uvec2& dimensions)
	{
		m_dimensions = dimensions;
	}

	void Deferred::set_color_texture(const std::string& name, Texture texture)
	{
		m_internal_textures[name] = std::move(texture);
		m_deferred_material.set_texture(name, &m_internal_textures[name]);
	}

	void Deferred::set_color_texture(const std::string& name, Texture* texture)
	{
		m_external_textures[name] = texture;
		m_deferred_material.set_texture(name, texture);
	}

	void Deferred::set_depth_texture(Texture texture)
	{
		m_g_depth = std::move(texture);
		set_depth_texture(&m_g_depth);
	}

	void Deferred::set_depth_texture(Texture* texture)
	{
		m_g_buffer_FBO->set_depth_buffer_texture(texture);
	}

	void Deferred::send_color_textures_to_framebuffer()
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
		m_g_buffer_FBO->push_back_color_buffer_textures(texture_vector);
	}
}