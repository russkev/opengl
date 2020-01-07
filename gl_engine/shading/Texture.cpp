#include "Texture.h"

#include <stdio.h>


namespace gl_engine
{
	// // ----- CONSTRUCTORS ----- // //
	/*

		Struct for loading multiple file types as textures
		Uses SDL library for image loading
		Help from Lazy Foo
		http://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index2.php

	*/
	Texture::Texture(const char* filepath) :
		m_surface(IMG_Load(filepath)),
		m_width(m_surface->w),
		m_height(m_surface->h)
		
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
			m_data = m_surface->pixels;
		}
	}

	Texture::Texture(const GLenum target) :
		m_target(target)
	{
		glGenTextures(1, &m_id);
	}

	Texture::Texture(const glm::vec3& color) :
		m_color{ (GLubyte)(color.r * 255), (GLubyte)(color.g * 255), (GLubyte)(color.b * 255), 255 }
	{
		m_internal_format = GL_RGB;
		m_format = GL_RGB;
		//m_color = {  };
		m_width = 1;
		m_height = 1;
		m_data = &m_color;
		m_type = GL_UNSIGNED_BYTE;
		glGenTextures(1, &m_id);
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

	// // ----- GETTERS ----- // //
	GLsizei Texture::width()
	{
		return m_width;
	}

	GLsizei Texture::height()
	{
		return m_height;
	}

	const GLuint Texture::id() const
	{
		return m_id;
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

} // namespace gl_engine