#include "pch.h"
#include "Texture.h"

#include <stdio.h>
#include <algorithm>

namespace glen
{
	// // ----- CONSTRUCTORS ----- // //
	/*

		Struct for loading multiple file types as textures
		Uses SDL library for image loading
		Help from Lazy Foo
		http://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index2.php

	*/
	Texture::Texture(const char* filepath) :
		m_name{ filepath },
		m_surface{ IMG_Load(filepath) },
		m_width{ m_surface->w },
		m_height{ m_surface->h }

	{
		if (has_alpha())
		{
			m_internal_format = GL_RGBA;
			m_format = GL_BGRA;
		}
		else
		{
			m_internal_format = GL_RGB;
			m_format = GL_BGR;
		}

		glGenTextures(1, &m_id);
		if (m_surface == NULL)
		{
			printf("WARNING: Loading \"%s\" failed. (%s)", filepath, IMG_GetError());
		}
		else
		{
			flip_surface();
			m_data = m_surface->pixels;
		}
	}

	Texture::Texture(const GLenum target) :
		m_name{ "Texture" },
		m_target{ target }
	{
		glGenTextures(1, &m_id);
	}

	Texture::Texture(const glm::vec3& color) :
		m_name{ "Constant Color Texture" },
		m_color{ (GLubyte)(color.r * 255), (GLubyte)(color.g * 255), (GLubyte)(color.b * 255), 255 },
		m_internal_format{ GL_RGB },
		m_format{ GL_RGB },
		m_width{ 1 },
		m_height{ 1 },
		m_data{ &m_color },
		m_type{ GL_UNSIGNED_BYTE },
		m_generate_mipmap{ false }
	{
		glGenTextures(1, &m_id);
	}

	Texture::Texture(const glm::vec4& color) :
		m_name{ "Constant Color Texture With Alpha" },
		m_color{ (GLubyte)(color.r * 255), (GLubyte)(color.g * 255), (GLubyte)(color.b * 255), (GLubyte)(color.a * 255) },
		m_internal_format{ GL_RGBA },
		m_format{ GL_RGBA },
		m_width{ 1 },
		m_height{ 1 },
		m_data{ &m_color },
		m_type{ GL_UNSIGNED_BYTE },
		m_generate_mipmap{ false }
	{
		glGenTextures(1, &m_id);
	}

	Texture::Texture(Texture&& other) :
		m_name{ std::exchange(other.m_name, "") },
		m_surface{ other.m_surface },
		m_color{ other.m_color },
		m_id{ std::exchange(other.m_id, 0) },
		m_width{ std::exchange(other.m_width, 0) },
		m_height{ std::exchange(other.m_height, 0) },
		m_target{ std::exchange(other.m_target, 0) },
		m_level{ std::exchange(other.m_level, 0) },
		m_internal_format{ std::exchange(other.m_internal_format, 0) },
		m_border{ std::exchange(other.m_border, 0) },
		m_format{ std::exchange(other.m_format, 0) },
		m_type{ std::exchange(other.m_type, 0) },
		m_data{ other.m_data },
		m_min_filter{ std::exchange(other.m_min_filter, 0) },
		m_mag_filter{ std::exchange(other.m_mag_filter, 0) },
		m_wrap_s{ std::exchange(other.m_wrap_s, 0) },
		m_wrap_t{ std::exchange(other.m_wrap_t, 0) },
		m_wrap_r{ std::exchange(other.m_wrap_r, 0) },
		m_border_color{ other.m_border_color[0], other.m_border_color[1], other.m_border_color[2], other.m_border_color[3] },
		m_generate_mipmap{ std::exchange(other.m_generate_mipmap, false) }
	{
		other.m_surface = NULL;
		other.m_data = NULL;
	}

	Texture& Texture::operator = (Texture&& other)
	{
		(*this).~Texture();
		return *new (this) Texture(std::move(other));
	}

	Texture::~Texture()
	{
		if (m_id != 0)
		{
			glDeleteTextures(1, &m_id);
		}
		if (m_surface)
		{
			SDL_FreeSurface(m_surface);
		}
		m_surface = NULL;
		m_id = 0;
	}


	// // ----- GENERAL METHODS ----- // //
	void Texture::process()
	{
		bind();

		if (m_target == GL_TEXTURE_2D)
		{
			process_uniform_2d();
		}
		else if (m_target == GL_TEXTURE_2D_ARRAY)
		{
			process_uniform_2d_array();
		}
		else if (m_target == GL_TEXTURE_CUBE_MAP)
		{
			process_cube_map();
		}

		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_min_filter);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_mag_filter);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_wrap_s);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_wrap_t);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_R, m_wrap_r);
		if (m_generate_mipmap)
		{
			glGenerateMipmap(m_target);
		}
		glTexParameterfv(m_target, GL_TEXTURE_BORDER_COLOR, m_border_color);

		unbind();

	}
	
	void Texture::process_uniform_2d()
	{
		glTexImage2D
		(
			m_target,
			m_level,
			m_internal_format,
			m_width,
			m_height,
			m_border,
			m_format,
			m_type,
			m_data
		);
	}

	void Texture::process_uniform_2d_array()
	{
		glTexImage3D(
			m_target,
			m_level,
			m_internal_format,
			m_width,
			m_height,
			1,
			m_border,
			m_format,
			m_type,
			m_data
		);
	}

	void Texture::process_cube_map()
	{
		for (GLuint i = 0; i < 6; ++i)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				m_level,
				m_internal_format,
				m_width,
				m_height,
				m_border,
				m_format,
				m_type,
				m_data);
		}
	}

	void Texture::flip_surface()
	{
		// Algorithm thanks to GitHub user wduminy
		// https://gist.github.com/wduminy/5859474

		SDL_Surface* result = SDL_CreateRGBSurface(
			m_surface->flags,
			m_surface->w,
			m_surface->h,
			m_surface->format->BytesPerPixel * 8,
			m_surface->format->Rmask,
			m_surface->format->Gmask,
			m_surface->format->Bmask,
			m_surface->format->Amask);

		const auto pitch = m_surface->pitch;
		const auto pixel_length = pitch * (m_surface->h - 1);
		
		auto input_pixels = static_cast<unsigned char*>(m_surface->pixels) + pixel_length;
		auto result_pixels = static_cast<unsigned char*>(result->pixels);

		for (auto line = 0; line < m_surface->h; ++line)
		{
			memcpy(result_pixels, input_pixels, pitch);
			input_pixels -= pitch;
			result_pixels += pitch;
		}
		std::swap(m_surface, result);
	}

	void Texture::bind(GLuint texture_unit)
	{
		glActiveTexture(GLenum(GL_TEXTURE0 + texture_unit));
		bind();
	}

	void Texture::bind()
	{
		glBindTexture(m_target, m_id);
	}

	void Texture::unbind()
	{
		glBindTexture(m_target, 0);
	}

	bool Texture::is_framebuffer()
	{
		return m_data == NULL;
	}

	// // ----- FACTORY ----- // //
	Texture Texture::create_16bit_rgba_null_texture(const GLenum target, const glm::uvec2& dimensions)
	{
		return create_16bit_rgba_null_texture("16 Bit RGBA Null Texture", target, dimensions);
	}

	Texture Texture::create_16bit_rgba_null_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions)
	{
		Texture texture{ target };
		texture.set_name(name);
		texture.set_width(dimensions.x);
		texture.set_height(dimensions.y);
		texture.set_internal_format(GL_RGBA16F);
		texture.set_format(GL_RGBA);
		texture.set_type(GL_FLOAT);
		texture.set_data(NULL);
		texture.set_mipmap(true);
		texture.set_min_filter(GL_NEAREST);
		texture.set_mag_filter(GL_NEAREST);
		texture.set_st_wrap(GL_CLAMP_TO_EDGE);

		texture.process();

		return texture;
	}

	Texture Texture::create_16bit_rgb_null_texture(const GLenum target, const glm::uvec2& dimensions)
	{
		return create_16bit_rgb_null_texture("16 Bit RGB Null Texture", target, dimensions);
	}

	Texture Texture::create_16bit_rgb_null_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions)
	{
		Texture texture{ target };
		texture.set_name(name);
		texture.set_width(dimensions.x);
		texture.set_height(dimensions.y);
		texture.set_internal_format(GL_RGB16F);
		texture.set_format(GL_RGB);
		texture.set_type(GL_FLOAT);
		texture.set_data(NULL);
		texture.set_mipmap(true);
		texture.set_min_filter(GL_NEAREST);
		texture.set_mag_filter(GL_NEAREST);
		texture.set_st_wrap(GL_CLAMP_TO_EDGE);

		texture.process();

		return texture;
	}

	Texture Texture::create_8bit_rgba_null_texture(const GLenum target, const glm::uvec2& dimensions)
	{
		return create_8bit_rgba_null_texture("8 Bit RGBA Null Texture", target, dimensions);
	}

	Texture Texture::create_8bit_rgba_null_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions)
	{
		Texture texture{ target };
		texture.set_name(name);
		texture.set_width(dimensions.x);
		texture.set_height(dimensions.y);
		texture.set_internal_format(GL_RGBA);
		texture.set_format(GL_RGBA);
		texture.set_type(GL_UNSIGNED_BYTE);
		texture.set_data(NULL);
		texture.set_mipmap(true);
		texture.set_min_filter(GL_NEAREST);
		texture.set_mag_filter(GL_NEAREST);
		texture.set_st_wrap(GL_CLAMP_TO_EDGE);

		texture.process();

		return texture;
	}

	Texture Texture::create_8bit_rgb_null_texture(const GLenum target, const glm::uvec2& dimensions)
	{
		return create_8bit_rgb_null_texture("8 Bit RGB Null Texture", target, dimensions);
	}

	Texture Texture::create_8bit_rgb_null_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions)
	{
		Texture texture{ target };
		texture.set_name(name);
		texture.set_width(dimensions.x);
		texture.set_height(dimensions.y);
		texture.set_internal_format(GL_RGB);
		texture.set_format(GL_RGB);
		texture.set_type(GL_UNSIGNED_BYTE);
		texture.set_data(NULL);
		texture.set_mipmap(true);
		texture.set_min_filter(GL_NEAREST);
		texture.set_mag_filter(GL_NEAREST);
		texture.set_st_wrap(GL_CLAMP_TO_EDGE);

		texture.process();

		return texture;
	}

	Texture Texture::create_depth_null_texture(const GLenum target, const glm::uvec2& dimensions)
	{
		return create_depth_null_texture("Depth Null Texture", target, dimensions);
	}

	Texture Texture::create_depth_null_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions)
	{
		Texture texture{ target };
		texture.set_name(name);
		texture.set_internal_format(GL_DEPTH_COMPONENT);
		texture.set_width(dimensions.x);
		texture.set_height(dimensions.y);
		texture.set_format(GL_DEPTH_COMPONENT);
		texture.set_type(GL_FLOAT);
		texture.set_data(NULL);
		texture.set_mipmap(true);
		texture.set_min_filter(GL_NEAREST);
		texture.set_mag_filter(GL_NEAREST);
		texture.set_st_wrap(GL_CLAMP_TO_EDGE);

		texture.process();

		return texture;
	}

	Texture Texture::create_stencil_texture(const GLenum target, const glm::uvec2& dimensions)
	{
		return create_stencil_texture("Stencil Texture", target, dimensions);
	}

	Texture Texture::create_stencil_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions)
	{
		Texture texture{ target };
		texture.set_name(name);
		texture.set_internal_format(GL_STENCIL_INDEX8);
		texture.set_width(dimensions.x);
		texture.set_height(dimensions.y);
		texture.set_format(GL_STENCIL_INDEX);
		texture.set_type(GL_UNSIGNED_BYTE);
		texture.set_data(NULL);
		texture.set_mipmap(true);
		texture.set_min_filter(GL_NEAREST);
		texture.set_mag_filter(GL_NEAREST);
		texture.set_st_wrap(GL_CLAMP_TO_EDGE);

		texture.process();

		return texture;
	}

	Texture Texture::create_depth_null_texture_for_shadow(const GLenum target, const glm::uvec2& dimensions)
	{
		return create_depth_null_texture_for_shadow("Depth Null Texture For Shadows", target, dimensions);
	}

	Texture Texture::create_depth_null_texture_for_shadow(const std::string& name, const GLenum target, const glm::uvec2& dimensions)
	{
		Texture texture{ target };
		texture.set_name(name);
		texture.set_width((GLsizei)dimensions.x);
		texture.set_height((GLsizei)dimensions.y);
		texture.set_internal_format(GL_DEPTH_COMPONENT);
		texture.set_format(GL_DEPTH_COMPONENT);
		texture.set_type(GL_FLOAT);
		texture.set_min_filter(GL_NEAREST);
		texture.set_mag_filter(GL_NEAREST);
		texture.set_mipmap(true);
		texture.set_st_wrap(GL_CLAMP_TO_BORDER);

		texture.process();

		return texture;
	}

	Texture Texture::create_square_noise_tile_texture(const GLenum target, const glm::uvec2& dimensions, const std::vector<glm::vec3>& data)
	{
		return create_square_noise_tile_texture("Square Noise Tile Texture", target, dimensions, data);
	}

	Texture Texture::create_square_noise_tile_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions, const std::vector<glm::vec3>& data)
	{
		Texture texture{ target };
		texture.set_name(name);
		texture.set_width((GLsizei)dimensions.x);
		texture.set_height((GLsizei)dimensions.y);
		texture.set_internal_format(GL_RGB16F);
		texture.set_format(GL_RGB);
		texture.set_data((void*)data.data());
		texture.set_type(GL_FLOAT);
		texture.set_min_filter(GL_NEAREST);
		texture.set_mag_filter(GL_NEAREST);
		texture.set_mipmap(true);
		texture.set_st_wrap(GL_REPEAT);

		texture.process();

		return texture;
	}

	Texture Texture::create_bw_null_texture(const GLenum target, const glm::uvec2& dimensions)
	{
		return create_bw_null_texture("Monochrome Null Texture", target, dimensions);
	}

	Texture Texture::create_bw_null_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions)
	{
		Texture texture{ target };
		texture.set_name(name);
		texture.set_width((GLsizei)dimensions.x);
		texture.set_height((GLsizei)dimensions.y);
		texture.set_internal_format(GL_RED);
		texture.set_format(GL_RGB);
		texture.set_type(GL_FLOAT);
		texture.set_data(NULL);
		texture.set_min_filter(GL_NEAREST);
		texture.set_mag_filter(GL_NEAREST);
		texture.set_mipmap(true);
		texture.set_st_wrap(GL_CLAMP_TO_EDGE);

		texture.process();

		return texture;
	}


	// // ----- GETTERS ----- // //
	const std::string Texture::name() const
	{
		return m_name;
	}


	const GLuint Texture::id() const
	{
		return m_id;
	}

	const GLsizei Texture::width() const
	{
		return m_width;
	}

	const GLsizei Texture::height() const
	{
		return m_height;
	}

	const GLint Texture::level() const 
	{
		return m_level;
	}

	const GLenum Texture::internal_format() const
	{
		return m_internal_format;
	}

	const GLint Texture::border() const 
	{
		return m_border;
	}

	const GLenum Texture::format() const 
	{
		return m_format;
	}

	const GLenum Texture::type() const 
	{
		return m_type;
	}

	const GLenum Texture::min_filter() const
	{
		return m_min_filter;
	}

	const GLenum Texture::mag_filter() const
	{
		return m_mag_filter;
	}

	const GLenum Texture::wrap_s() const
	{
		return m_wrap_s;
	}

	const GLenum Texture::wrap_t() const
	{
		return m_wrap_t;
	}

	const GLenum Texture::wrap_r() const
	{
		return m_wrap_r;
	}

	const SDL_Surface* Texture::surface() const
	{
		return m_surface;
	}

	const glm::tvec4<GLubyte> Texture::color() const
	{
		return m_color;
	}

	const GLenum Texture::target() const
	{
		return m_target;
	}

	bool Texture::has_alpha()
	{
		if (m_surface)
		{
			return m_surface->format->BytesPerPixel == 4;
		}
		return false;
	}

	void* Texture::data()
	{
		if (m_surface)
		{
			return m_surface->pixels;
		}
		return false;
	}

	// // ----- SETTERS ----- // //
	void Texture::set_name(const std::string& name)
	{
		m_name = name;
	}

	void Texture::set_new_id(GLuint id)
	{
		m_id = id;
		bind();
	}

	void Texture::set_target(const GLenum target)
	{
		m_target = target;
	}

	void Texture::set_width(const GLsizei width)
	{
		m_width = width;
	}

	void Texture::set_height(const GLsizei height)
	{
		m_height = height;
	}

	void Texture::set_internal_format(const GLint internal_format)
	{
		m_internal_format = internal_format;
	}

	void Texture::set_format(const GLenum format)
	{
		m_format = format;
	}

	void Texture::set_type(const GLenum type)
	{
		m_type = type;
	}

	void Texture::set_data(void* data)
	{
		m_data = data;
	}

	void Texture::set_min_filter(const GLenum min_filter)
	{
		m_min_filter = min_filter;
	}

	void Texture::set_mag_filter(const GLenum max_filter)
	{
		m_mag_filter = max_filter;
	}

	void Texture::set_st_wrap(const GLenum wrap)
	{
		m_wrap_s = wrap;
		m_wrap_t = wrap;
		m_wrap_r = wrap;
	}

	void Texture::set_mipmap(const bool value)
	{
		m_generate_mipmap = value;
	}

	void Texture::set_border_color(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a)
	{
		m_border_color[0] = r;
		m_border_color[1] = g;
		m_border_color[2] = b;
		m_border_color[3] = a;
	}


	void Texture::resize(const GLsizei width, const GLsizei height)
	{
		m_width = width;
		m_height = height;
	}

}