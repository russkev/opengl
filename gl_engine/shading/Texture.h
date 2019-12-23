#ifndef GL_ENGINE_SHADING_TEXTURE_H
#define GL_ENGINE_SHADING_TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>


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
		GLuint			m_id;
		GLsizei			m_width;
		GLsizei			m_height;

		GLenum			m_target = GL_TEXTURE_2D;	//Type of texture
		GLint			m_level = 0;				// Mipmap base level
		GLint			m_internal_format = GL_RGB;	// Number and size of color components
		GLint			m_border = 0;				// Size of the border
		GLenum			m_format = GL_RGB;			// Format of the pixel data
		GLenum			m_type = GL_UNSIGNED_BYTE;	// The data type of each pixel
		void*			m_data = NULL;				// Pointer to the actual data

		GLenum			m_min_filter = GL_LINEAR_MIPMAP_LINEAR;
		GLenum			m_mag_filter = GL_LINEAR;
		GLenum			m_wrap_s = GL_REPEAT;
		GLenum			m_wrap_t = GL_REPEAT;

		bool			m_generate_mipmap = true;



		// // ----- CONSTRUCTOR ----- // //
	public:
		// Create a texture from an image file
		Texture(const char* filename);

		// Create an internal texture
		Texture(GLuint width, GLuint height, GLenum format, GLenum type, void* data);
		
		// // ----- GENERAL ----- // //
		void upload_texture();
		void bind(GLuint texture_unit = 0);
		void unbind();

		// // ----- GETTERS ----- // //
	public:
		GLsizei width();
		GLsizei height();
		const GLuint tex_id() const;
		bool hasAlpha();
		void* data();
	};
} // namespace gl_engine

#endif