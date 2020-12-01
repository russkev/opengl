#ifndef GL_ENGINE_SHADING_TEXTURE_H
#define GL_ENGINE_SHADING_TEXTURE_H

#include <vector>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#undef main
#define SDL_MAIN_HANDLED
#include <SDL_image.h>
#undef main
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace glen
{
	/*
	
		A texture is used to prepare sampler objects for shaders
	
	*/
	struct Texture
	{
		// // ----- CONSTRUCTORS ----- // //
	public:
		Texture();
		// Create a texture from an image file
		Texture(const char* file_name);
		Texture(const char* file_name, const bool is_srgb);

		// Create an internal texture
		Texture(const GLenum target);

		// Create color solid color texture
		Texture(const glm::vec3& color);
		Texture(const glm::vec4& color);

		Texture(const Texture& other) = delete;
		Texture(Texture&& other) noexcept;
		Texture& operator = (const Texture& other) = delete;
		Texture& operator = (Texture&& other) noexcept;
		~Texture();


		
		// // ----- GENERAL ----- // //
		void process();
		void bind();
		void bind(GLuint texture_unit);
		void unbind();
		bool is_framebuffer();
	private:
		void process_uniform_2d();
		void process_uniform_2d_array();
		void process_cube_map();
		void process_textured_cube_map();
		void flip_surface();

		// // ----- FACTORY ----- // //
	public:
		static Texture create_16bit_rgba_null_texture(const GLenum target, const glm::uvec2& dimensions);
		static Texture create_16bit_rgba_null_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions);
		static Texture create_16bit_rgb_null_texture(const GLenum target, const glm::uvec2& dimensions);
		static Texture create_16bit_rgb_null_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions);
		static Texture create_8bit_rgba_null_texture(const GLenum target, const glm::uvec2& dimensions);
		static Texture create_8bit_rgba_null_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions);
		static Texture create_8bit_rgb_null_texture(const GLenum target, const glm::uvec2& dimensions);
		static Texture create_8bit_rgb_null_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions);
		static Texture create_depth_null_texture(const GLenum target, const glm::uvec2& dimensions);
		static Texture create_depth_null_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions);
		static Texture create_stencil_texture(const GLenum target, const glm::uvec2& dimensions);
		static Texture create_stencil_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions);
		static Texture create_depth_null_texture_for_shadow(const GLenum target, const glm::uvec2& dimensions);
		static Texture create_depth_null_texture_for_shadow(const std::string& name, const GLenum target, const glm::uvec2& dimensions);
		static Texture create_square_noise_tile_texture(const GLenum target, const glm::uvec2& dimensions, const std::vector<glm::vec3>& data);
		static Texture create_square_noise_tile_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions, const std::vector<glm::vec3>& data);
		static Texture create_bw_null_texture(const GLenum target, const glm::uvec2& dimensions);
		static Texture create_bw_null_texture(const std::string& name, const GLenum target, const glm::uvec2& dimensions);

		static Texture create_cubemap_texture(const std::vector<const char*> face_paths);

		// // ----- GETTERS ----- // //
	public:
		const std::string name() const;
		const GLuint id() const;
		const GLsizei width() const;
		const GLsizei height() const;

		const GLenum target() const;
		const GLint level() const;
		const GLenum internal_format() const;
		const GLint border() const;
		const GLenum format() const;
		const GLenum type() const;

		const GLenum min_filter() const;
		const GLenum mag_filter() const;
		const GLenum wrap_s() const;
		const GLenum wrap_t() const;
		const GLenum wrap_r() const;

		const SDL_Surface* surface() const;
		const std::vector<SDL_Surface*> surfaces() const;
		const glm::tvec4<GLubyte> color() const;

		bool has_alpha();

		// // ----- SETTERS ----- // //
		void set_name(const std::string& name);
		void set_new_id(const GLuint id);
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
		void add_surface(SDL_Surface* surface);
		void resize(const GLsizei width, const GLsizei height);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		std::string					m_name;
		std::vector<SDL_Surface*>	m_surfaces;
		glm::tvec4<GLubyte>			m_color{ 0, 0, 0, 0 };

		GLuint			m_id = 0;
		GLsizei			m_width = 0;
		GLsizei			m_height = 0;

		GLenum			m_target = GL_TEXTURE_2D;	// Type of texture
		GLint			m_level = 0;				// Mipmap base level
		GLenum			m_internal_format = GL_RGB;	// Number and size of color components
		GLint			m_border = 0;				// Size of the border
		GLenum			m_format = GL_RGB;			// Format of the pixel data
		GLenum			m_type = GL_UNSIGNED_BYTE;	// The data type of each pixel
		void*			m_data = NULL;				// Pointer to the actual data
													//		If multiple surfaces are loaded, this will
													//		point to only the data in the front of the
													//		vector of surfaces

		GLenum			m_min_filter = GL_LINEAR_MIPMAP_LINEAR;
		GLenum			m_mag_filter = GL_LINEAR;
		GLenum			m_wrap_s = GL_REPEAT;
		GLenum			m_wrap_t = GL_REPEAT;
		GLenum			m_wrap_r = GL_REPEAT;
		GLfloat			m_border_color[4] = { 1.0f, 0.0f, 0.0f, 0.0f };

		bool			m_generate_mipmap = true;
	};
}

#endif