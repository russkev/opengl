#ifndef GL_ENGINE_SHADING_TEXTURE_H
#define GL_ENGINE_SHADING_TEXTURE_H

namespace glen
{
	/*!
	
		A texture is used to prepare sampler objects for shaders
	
	*/
	struct Texture
	{
		// // ----- CONSTRUCTORS ----- // //
	public:
		/*!
		 * @brief Constructor
		*/
		Texture();
		
		/*!
		 * @brief Create a texture from an image file located at `file_name`.
		*/
		Texture(const char* file_name);

		/*!
		 * @brief Create a texture from an image file located at `file_name`.
		 * @param is_srgb If this flag is False, image is assumed to be linear.
		*/
		Texture(const char* file_name, const bool is_srgb);

		/*!
		* @brief Construct an internal texture.
		*/
		Texture(const GLenum target);

		/*!
		 * @brief Construct color solid color texture.
		 */
		Texture(const glm::vec3& color);

		/*!
		 * @brief Construct color solid color texture with an alpha.
		 */
		Texture(const glm::vec4& color);

		/*!
		 * @brief Use move constructor instead of copy constructor.
		*/
		Texture(const Texture& other) = delete;

		/*!
		 * @brief Move constructor.
		*/
		Texture(Texture&& other) noexcept;

		/*!
		 * @brief Use move assign instead of copy assign.
		*/
		Texture& operator = (const Texture& other) = delete;

		/*!
		 * @brief Move assign.
		*/
		Texture& operator = (Texture&& other) noexcept;
		~Texture();


		
		// // ----- GENERAL ----- // //

		/*!
		 * @brief Tell OpenGL about all properties of the texture
		*/
		void process();

		/*!
		 * @brief Make this texture 'active' in relation to OpenGL
		*/
		void bind();

		/*!
		 * @brief Make the texture associated with `texture_unit` active in relation to OpenGL.
		*/
		void bind(GLuint texture_unit);

		/*!
		 * @brief Unbind this texture.
		*/
		void unbind();

		/*!
		 * @return True if the texture contains no data.
		*/
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
		/*!
		 * @return The texture name
		*/
		const std::string name() const;

		/*!
		 * @return The GL Texture ID.
		*/
		const GLuint id() const;

		/*!
		 * @return The width of the texture
		*/
		const GLsizei width() const;

		/*!
		 * @return The height of the texture
		*/
		const GLsizei height() const;

		/*!
		 * @return The OpenGL target (e.g. `GL_TEXTURE_2D`)
		*/
		const GLenum target() const;

		/*!
		 * @return The mipmap level
		*/
		const GLint level() const;

		/*!
		 * @return The internal OpenGL format (e.g. `GL_RGB`)
		*/
		const GLenum internal_format() const;

		/*!
		 * @return The size in pixels of the border
		*/
		const GLint border() const;

		/*!
		 * @return The format of the texture (e.g. `GL_RGB)
		*/
		const GLenum format() const;

		/*!
		 * @return The data type of the pixels (e.g. `GL_UNSIGNED_BYTE`)
		*/
		const GLenum type() const;

		/*!
		 * @return The filtering method for when the texture is smaller than the screen (e.g. `GL_LINEAR_MIPMAP_LINEAR`)
		*/
		const GLenum min_filter() const;

		/*!
		 * @return The filtering method for when the texture is larger than the screen (e.g. `GL_LINEAR`)
		*/
		const GLenum mag_filter() const;

		/*!
		 * @return What to do at the edges of the texture in the s / u direction (e.g. `GL_REPEAT`)
		*/
		const GLenum wrap_s() const;

		/*!
		 * @return What to do at the edges of the texture in the t / v direction (e.g. `GL_REPEAT`)
		*/
		const GLenum wrap_t() const;

		/*!
		 * @return What to do at the edges of the texture in the r direction (e.g. `GL_REPEAT`)
		*/
		const GLenum wrap_r() const;


		/*!
		 * @return The SDL surface being used to hold the texture information
		*/
		const SDL_Surface* surface() const;

		/*!
		 * @return The list of SDL surfaces being used to hold all textures
		*/
		const std::vector<SDL_Surface*> surfaces() const;

		/*!
		 * @return The solid color being used.
		*/
		const glm::tvec4<GLubyte> color() const;

		/*!
		 * @return True if texture is making use of an alpha channel.
		*/
		bool has_alpha();


		// // ----- SETTERS ----- // //

		/*!
		 * @brief Set the texture name.
		*/
		void set_name(const std::string& name);

		/*!
		 * @brief Set the texture ID.
		*/
		void set_new_id(const GLuint id);

		/*!
		 * @brief Set the target (e.g. `GL_TEXTURE_2D`).
		*/
		void set_target(const GLenum target);

		/*!
		 * @brief Set the width in pixels of the texture.
		*/
		void set_width(const GLsizei width);

		/*!
		 * @brief Set the height in pixels of the texture.
		*/
		void set_height(const GLsizei height);

		/*!
		 * @brief Set the internal format (e.g. `GL_RGB`).
		*/
		void set_internal_format(const GLint internal_format);

		/*!
		 * @brief Set the format (e.g. `GL_RGB`).
		*/
		void set_format(const GLenum format);

		/*!
		 * @brief Set the pixel data type (e.g. `GL_UNSIGNED_BYTE`).
		*/
		void set_type(const GLenum type);

		/*!
		 * @brief Set the actual texture data.
		*/
		void set_data(void* data);

		/*!
		 * @brief Set the filtering method for when the texture is smaller than the screen (e.g. `GL_LINEAR_MIPMAP_LINEAR`)
		*/
		void set_min_filter(const GLenum min_filter);

		/*!
		 * @brief Set the filtering method for when the texture is larger than the screen (e.g. `GL_LINEAR`)
		*/
		void set_mag_filter(const GLenum max_filter);

		/*!
		 * @brief Set the behaviour when the uv/uvw texture space has been exceeded (e.g. GL_REPEAT).
		*/
		void set_st_wrap(const GLenum wrap);

		/*!
		 * @brief Set true if you want to generate mipmaps. This usually make textures look better on screen.
		*/
		void set_mipmap(const bool value);

		/*!
		 * @brief Set the `r`, `g`, `b`, `a` border color values/
		*/
		void set_border_color(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a);

		/*!
		 * @brief Add a SDL surface to this texture.
		*/
		void add_surface(SDL_Surface* surface);

		/*!
		 * @brief Change the dimensions of this texture to `width` x `height`.
		*/
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