#ifndef GL_ENGINE_SHADING_TEXTURE_H
#define GL_ENGINE_SHADING_TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>
#include <glm/glm.hpp>


namespace gl_engine
{
	/*
	
		A texture is used to prepare sampler objects for shaders
	
	*/
	struct Texture
	{
		// // ----- CONSTRUCTORS ----- // //
	public:
		Texture() {};

		// Create a texture from an image file
		Texture(const char* file_name);

		// Create an internal texture
		Texture(const GLenum target);

		// Create color solid color texture
		Texture(const glm::vec3& color);
		Texture(const glm::vec4& color);
		
		// // ----- GENERAL ----- // //
		void process();
		void bind();
		void bind(GLuint texture_unit);
		void unbind();
	private:
		void process_uniform_2d();
		void process_uniform_2d_array();
		void process_cube_map();
		void flip_surface();

		// // ----- GETTERS ----- // //
	public:
		GLsizei width();
		GLsizei height();
		const GLuint id() const;
		bool has_alpha();
		void* data();


		// // ----- SETTERS ----- // //

		void set_target(const GLenum target);
		void set_width(const GLsizei width);
		void set_height(const GLsizei height);
		void set_internal_format(const GLint internal_format);
		void set_format(const GLenum format);
		void set_type(const GLenum type);
		void set_data(void* data);
		void set_min_filter(const GLenum min_filter);
		void set_mag_filter(const GLenum max_filter);
		void set_st_wrap(const GLenum wrap);
		void set_mipmap(const bool value);
		void set_border_color(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a);
		void resize(const GLsizei width, const GLsizei height);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		SDL_Surface*	m_surface = NULL;
		glm::tvec4<GLubyte>	m_color;

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
		GLenum			m_wrap_r = GL_REPEAT;
		GLfloat			m_border_color[4] = { 1.0f, 0.0f, 0.0f, 0.0f };

		bool			m_generate_mipmap = true;
	};
} // namespace gl_engine

#endif