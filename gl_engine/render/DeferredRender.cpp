#include "pch.h"
#include "DeferredRender.h"

namespace glen
{
	// // ----- CONSTRUCTOR ----- // //
	DeferredRender::DeferredRender(const GLenum target, const glm::uvec2& dimensions) :
	//	m_target{ target },
	//	m_dimensions{ dimensions }
	//{}
		m_dimensions{ dimensions }
		//m_g_position{ Texture::create_16bit_rgb_null_texture(target, m_dimensions) },
		//m_g_normal{ Texture::create_16bit_rgb_null_texture(target, m_dimensions) },
		//m_g_color_spec{ Texture::create_8bit_rgba_null_texture(target, m_dimensions) },
		//m_g_depth{ Texture::create_depth_null_texture(target, m_dimensions) }
	{
		//m_g_buffer_FBO.push_back_color_buffer_textures(std::vector<const Texture*>{
		//	&m_g_position, &m_g_normal, &m_g_color_spec	});

		//m_g_buffer_FBO.set_depth_buffer_texture(&m_g_depth);

		//m_deferred_material.set_texture(BlinnDeferredMaterial::k_g_position, &m_g_position);
		//m_deferred_material.set_texture(BlinnDeferredMaterial::k_g_normal, &m_g_normal);
		//m_deferred_material.set_texture(BlinnDeferredMaterial::k_g_diffuse_spec, &m_g_color_spec);
	}

	DeferredRender::DeferredRender(DeferredRender&& other) :
		m_target{ other.m_target },
		m_dimensions{ std::exchange(other.m_dimensions, glm::uvec2{ 0u }) },
		m_g_buffer_FBO{ std::move(other.m_g_buffer_FBO) },
		m_deferred_material{ std::move(other.m_deferred_material)},
		m_textures{ std::exchange(other.m_textures, {}) },
		m_g_depth{std::move(other.m_g_depth)},
		m_deferred_mesh{ std::move(other.m_deferred_mesh) },
		m_deferred_mesh_node{ other.m_deferred_mesh_node.name(), &m_deferred_mesh, &m_deferred_material }
	{
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
	void DeferredRender::setup_blinn_deferred(const GLenum target, const glm::uvec2& dimensions)
	{
		m_dimensions = dimensions;
		m_target = target;

		set_color_texture(BlinnDeferredMaterial::k_g_position, Texture::create_16bit_rgb_null_texture(target, m_dimensions));
		set_color_texture(BlinnDeferredMaterial::k_g_normal, Texture::create_16bit_rgb_null_texture(target, m_dimensions));
		set_color_texture(BlinnDeferredMaterial::k_g_diffuse_spec, Texture::create_8bit_rgba_null_texture(target, m_dimensions));
		set_depth_texture(Texture::create_depth_null_texture(target, m_dimensions));

		send_color_textures_to_framebuffer();
	}

	//DeferredRender DeferredRender::create_blinn_deferred(const GLenum target, const glm::uvec2& dimensions)
	//{
	//	//DeferredRender deferred_render;
	//	//deferred_render.set_dimensions(dimensions);
	//	//deferred_render.set_target(target);
	//	//deferred_render.
	//	//m_g_position = Texture::create_16bit_rgb_null_texture(target, m_dimensions);
	//	//m_g_normal = Texture::create_16bit_rgb_null_texture(target, m_dimensions);
	//	//m_g_color_spec = Texture::create_8bit_rgba_null_texture(target, m_dimensions);
	//	//m_g_depth = Texture::create_depth_null_texture(target, m_dimensions);
	//	//m_g_buffer_FBO.push_back_color_buffer_textures(std::vector<const Texture*>{
	//	//	&m_g_position, &m_g_normal, &m_g_color_spec	});

	//	//m_g_buffer_FBO.set_depth_buffer_texture(&m_g_depth);

	//	//m_deferred_material.set_texture(BlinnDeferredMaterial::k_g_position, &m_g_position);
	//	//m_deferred_material.set_texture(BlinnDeferredMaterial::k_g_normal, &m_g_normal);
	//	//m_deferred_material.set_texture(BlinnDeferredMaterial::k_g_diffuse_spec, &m_g_color_spec);
	//}

	//DeferredRender DeferredRender::create_blinn_deferred(const GLenum target, const glm::uvec2& dimensions)
	//{
	//	DeferredRender deferred_render(target, dimensions);
	//	Texture g_position{		Texture::create_16bit_rgb_null_texture(target, dimensions) };
	//	Texture g_normal{		Texture::create_16bit_rgb_null_texture(target, dimensions) };
	//	Texture g_color_spec{	Texture::create_8bit_rgba_null_texture(target, dimensions) };
	//	Texture g_depth{		Texture::create_depth_null_texture(target, dimensions) };

	//	deferred_render.framebuffer()->push_back_color_buffer_textures( std::vector<const Texture*>{
	//		&g_position, &g_normal, &g_color_spec	});
	//	deferred_render.framebuffer()->set_depth_buffer_texture(&g_depth);

	//	deferred_render.material()->set_texture(BlinnDeferredMaterial::k_g_position, &g_position);
	//	deferred_render.material()->set_texture(BlinnDeferredMaterial::k_g_normal, &g_normal);
	//	deferred_render.material()->set_texture(BlinnDeferredMaterial::k_g_diffuse_spec, &g_color_spec);

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
		return &m_textures[name];
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
		m_textures[name] = std::move(texture);
		set_color_texture(name, &m_textures[name]);
	}

	void DeferredRender::set_color_texture(const std::string& name, Texture* texture)
	{
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
		for (const auto & texture_pair : m_textures)
		{
			texture_vector.push_back(&texture_pair.second);
		}
		m_g_buffer_FBO.push_back_color_buffer_textures(texture_vector);
	}
}