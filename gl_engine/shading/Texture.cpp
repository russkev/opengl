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
		GLenum format = hasAlpha() ? GL_RGBA : GL_RGB;
		m_format = format;
		m_internal_format = format;

		glGenTextures(1, &m_id);
		if (m_surface == NULL)
		{
			printf("WARNING: Loading \"%s\" failed. (%s)", filepath, IMG_GetError());
		}
		else
		{
			m_data = m_surface->pixels;
			//upload_texture();
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
		//upload_texture();
	}

	void Texture::upload_texture()
	{
		//bind();
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGB,
			m_width,
			m_height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			data()
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glGenerateMipmap(GL_TEXTURE_2D);

		//unbind();

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
} // namespace gl_engine