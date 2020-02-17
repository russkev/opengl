#include "pch.h"
#include "Deferred.h"

#include "utils/ScalarUtils.h"

namespace glen
{
	// // ----- CONSTRUCTOR ----- // //
	Deferred::Deferred(const GLenum target, Framebuffer* g_buffer, Material* material, const glm::uvec2& dimensions) :
		m_target{ target },
		m_dimensions{ dimensions },
		m_g_buffer_FBO{ g_buffer },
		m_material{ std::move(material) },
		m_mesh_node{ "Screen Node", PostEffect::mesh(), m_material }
	{}

	Deferred::Deferred(Deferred&& other) :
		m_target{ other.m_target },
		m_dimensions{ std::exchange(other.m_dimensions, glm::uvec2{ 0u }) },
		m_external_textures{ std::move(other.m_external_textures) },
		m_material{ std::move(other.m_material) },
		m_mesh_node{ other.m_mesh_node.name(), PostEffect::mesh(), m_material }
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
		m_material->update_view(camera_node, NULL);
	}

	void Deferred::draw()
	{
		m_mesh_node.draw();
	}

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
		return m_material;
	}

	MeshNode* Deferred::mesh_node()
	{
		return &m_mesh_node;
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
		m_material->set_texture(name, &m_internal_textures[name]);
	}

	void Deferred::set_color_texture(const std::string& name, Texture* texture)
	{
		m_external_textures[name] = texture;
		m_material->set_texture(name, texture);
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

	BlinnDeferred::BlinnDeferred(const GLenum target, Framebuffer* g_buffer, const glm::uvec2& dimensions) :
		Deferred{ target, g_buffer, &m_material, dimensions }
	{
		set_color_texture(BlinnDeferredMaterial::k_g_position, Texture::create_16bit_rgb_null_texture(target, dimensions));
		set_color_texture(BlinnDeferredMaterial::k_g_normal, Texture::create_16bit_rgb_null_texture(target, dimensions));
		set_color_texture(BlinnDeferredMaterial::k_g_diffuse_spec, Texture::create_8bit_rgba_null_texture(target, dimensions));
		set_depth_texture(Texture::create_depth_null_texture(target, dimensions));

		send_color_textures_to_framebuffer();
	}

	//AO_GBufferDeferred::AO_GBufferDeferred(const GLenum target, Framebuffer* g_buffer, const glm::uvec2& dimensions) :
	//	Deferred{ target, g_buffer, &m_material, dimensions }
	//{
	//	set_color_texture(AO_GBufferMaterial::k_g_position, Texture::create_16bit_rgb_null_texture(target, dimensions));
	//	set_color_texture(AO_GBufferMaterial::k_g_normal, Texture::create_16bit_rgb_null_texture(target, dimensions));
	//	set_color_texture(AO_GBufferMaterial::k_g_diffuse, Texture::create_8bit_rgb_null_texture(target, dimensions));
	//	set_depth_texture(Texture::create_depth_null_texture(target, dimensions));

	//	send_color_textures_to_framebuffer();
	//}

	//void AO_GBufferDeferred::init_kernal()
	//{
	//	GLuint num_samples = 64;

	//	for (GLuint i = 0; i < num_samples; ++i)
	//	{
	//		glm::vec3 sample{
	//			m_random_floats(m_generator) * 2.0f - 1.0f,
	//			m_random_floats(m_generator) * 2.0f - 1.0f,
	//			m_random_floats(m_generator)
	//		};

	//		sample = glm::normalize(sample);
	//		sample *= m_random_floats(m_generator);
	//		sample *= increase_nearby_samples(i, num_samples);

	//		m_kernal.push_back(sample);
	//	}
	//}

	//float AO_GBufferDeferred::increase_nearby_samples(const GLuint i, const GLuint num_samples)
	//{
	//	GLfloat scale = (GLfloat)i / (GLfloat)num_samples;
	//	scale = ScalarUtils::lerp(0.1f, 1.0f, scale * scale);
	//	return scale;
	//}

	//void AO_GBufferDeferred::init_noise()
	//{
	//	GLuint num_fragments = m_noise_tile_dimensions.x * m_noise_tile_dimensions.y;
	//	for (unsigned int i = 0; i < num_fragments; ++i)
	//	{
	//		glm::vec3 noise_fragment(
	//			m_random_floats(m_generator) * 2.0f - 1.0f,
	//			m_random_floats(m_generator) * 2.0f - 1.0f,
	//			0.0f);
	//		m_noise_tile.push_back(noise_fragment);
	//	}
	//	m_noise_tile_texture = Texture::create_square_noise_tile_texture(GL_TEXTURE_2D, m_noise_tile_dimensions, m_noise_tile);
	//}

	//AO_Deferred::AO_Deferred(const GLenum target, Framebuffer* ao_buffer, const glm::uvec2& dimensions) :
	//	Deferred{ target, ao_buffer, &m_material, dimensions }
	//{}
}