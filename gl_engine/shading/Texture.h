#ifndef GL_ENGINE_SHADING_TEXTURE_H
#define GL_ENGINE_SHADING_TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>

struct Texture
{
	// // ----- MEMBER ATTRIBUTES ----- // //
private:
	SDL_Surface*	m_surface = NULL;
	const uint16_t	m_id;
	static uint16_t	m_next_id;


	// // ----- CONSTRUCTOR ----- // //
public:
	Texture(const char* filename);

	// // ----- GETTERS ----- // //
	size_t width();
	size_t height();
	const size_t tex_id() const;
	bool hasAlpha();
	void* data();
};


#endif