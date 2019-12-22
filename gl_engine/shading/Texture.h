#ifndef GL_ENGINE_SHADING_TEXTURE_H
#define GL_ENGINE_SHADING_TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>

namespace gl_engine
{
	/*
	
		A texture is an image that is procured externally
	
	*/
	struct Texture
	{
		// // ----- MEMBER ATTRIBUTES ----- // //
	private:
		SDL_Surface*	m_surface = NULL;
		const uint16_t	m_id;
		static uint16_t	m_next_id;

		size_t			m_width;
		size_t			m_height;


		// // ----- CONSTRUCTOR ----- // //
	public:
		// Create a texture from an image file
		Texture(const char* filename);

		// Create an internal texture
		Texture(size_t width, size_t height);

		// // ----- GETTERS ----- // //
		size_t width();
		size_t height();
		const size_t tex_id() const;
		bool hasAlpha();
		void* data();
	};
} // namespace gl_engine

#endif