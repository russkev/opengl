#include <stdio.h>

#include "Texture.h"

namespace gl_engine
{
	/*

		Struct for loading multiple file types as textures
		Uses SDL library for image loading
		Help from Lazy Foo
		http://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index2.php

	*/
	uint16_t Texture::m_next_id = 0;

	Texture::Texture(const char* filepath) :
		m_surface(IMG_Load(filepath)),
		m_id(m_next_id++),
		m_width(m_surface->w),
		m_height(m_surface->h)
	{
		if (m_surface == NULL)
		{
			printf("WARNING: Loading \"%s\" failed. (%s)", filepath, IMG_GetError());
		}
	}

	Texture::Texture(size_t width, size_t height) :
		m_id(m_next_id++),
		m_width(width),
		m_height(height)
	{}

	size_t Texture::width()
	{
		return m_width;
	}

	size_t Texture::height()
	{
		return m_height;
	}

	const size_t Texture::tex_id() const
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