#include "Texture.h"

#include <stdio.h>


namespace gl_engine
{
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
		if (hasAlpha())
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

	Texture::Texture(GLuint width, GLuint height, GLenum format, GLenum type, void* data) :
		m_width(width),
		m_height(height),
		m_internal_format(format),
		m_format(format),
		m_type(type),
		m_data(data)
	{
		glGenTextures(1, &m_id);
	}

	void Texture::upload_texture()
	{
		bind();

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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_mag_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_t);
		if (m_generate_mipmap)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		unbind();

	}

	void Texture::bind(GLuint texture_unit)
	{
		glActiveTexture(GLenum(GL_TEXTURE0 + texture_unit));
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void Texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLsizei Texture::width()
	{
		return m_width;
	}

	GLsizei Texture::height()
	{
		return m_height;
	}

	const GLuint Texture::tex_id() const
	{
		return m_id;
	}

	bool Texture::hasAlpha()
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
	void Texture::set_internalFormat(const GLint internal_format)
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

	void Texture::set_date(void* data)
	{
		m_data = data;
	}

	void Texture::set_minFilter(const GLenum min_filter)
	{
		m_min_filter = min_filter;
	}

	void Texture::set_magFilter(const GLenum max_filter)
	{
		m_mag_filter = max_filter;
	}

	void Texture::set_st_wrap(const GLenum wrap)
	{
		m_wrap_s = wrap;
		m_wrap_t = wrap;
	}

	void Texture::set_mipmap(const bool value)
	{
		m_generate_mipmap = value;
	}
} // namespace gl_engine