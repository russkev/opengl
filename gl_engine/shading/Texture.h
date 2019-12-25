#ifndef GL_ENGINE_SHADING_TEXTURE_H
#define GL_ENGINE_SHADING_TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>


namespace gl_engine
{
	/*
	
		A texture is used to prepare sampler objects for shaders
	
	*/
	struct Texture
	{
		// // ----- MEMBER ATTRIBUTES ----- // //
	private:
		SDL_Surface*	m_surface = NULL;
		GLuint			m_id = 0;
		GLsizei			m_width = 0;
		GLsizei			m_height = 0;

		GLenum			m_target = GL_TEXTURE_2D;	// Type of texture
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
		Texture() {};

		// Create a texture from an image file
		Texture(const char* filename);

		// Create an internal texture
		Texture(GLuint width, GLuint height, GLenum format, GLenum type, void* data);
		
		// // ----- GENERAL ----- // //
		void process();
		void bind();
		void bind(GLuint texture_unit);
		void unbind();

		// // ----- GETTERS ----- // //
	public:
		GLsizei width();
		GLsizei height();
		const GLuint id() const;
		bool hasAlpha();
		void* data();


		// // ----- SETTERS ----- // //
		void set_internalFormat(const GLint internal_format);
		void set_format(const GLenum format);
		void set_type(const GLenum type);
		void set_date(void* data);
		void set_minFilter(const GLenum min_filter);
		void set_magFilter(const GLenum max_filter);
		void set_st_wrap(const GLenum wrap);
		void set_mipmap(const bool value);
		void resize(const GLsizei width, const GLsizei height);

	};
} // namespace gl_engine

#endif