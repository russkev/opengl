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
		m_id(m_next_id++)
	{
		//m_id = 5;
		//m_id = Texture::m_next_tex_id++;

		if (m_surface == NULL)
		{
			printf("WARNING: Loading \"%s\" failed. (%s)", filepath, IMG_GetError());
		}
	}

	size_t Texture::width()
	{
		return m_surface->w;
	}

	size_t Texture::height()
	{
		return m_surface->h;
	}

	const size_t Texture::tex_id() const
	{
		return m_id;
	}

	bool Texture::hasAlpha()
	{
		return m_surface->format->BytesPerPixel == 4;
	}

	void* Texture::data()
	{
		return m_surface->pixels;
	}
} // namespace gl_engine