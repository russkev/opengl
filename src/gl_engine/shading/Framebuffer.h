#ifndef GL_ENGINE_SHADING_FRAMEBUFFER_H
#define GL_ENGINE_SHADING_FRAMEBUFFER_H

namespace glen
{
	// // ----- FORWARD DECLERATION ----- // //
	struct Texture;

	/*!
	 
		Framebuffers are used to define a GL Framebuffer to render to. 
		This has the option of being something other than the main window.

	*/
	struct Framebuffer
	{
	public:
		// // ----- CONSTRUCTORS ----- // //
		
		/*!
		 * @brief Constructor
		*/
		Framebuffer();

		/*!
		 * @brief Constructor with custom GL target. 
		 * By default the target is `GL_FRAMEBUFFER`.
		 * @param target 
		*/
		Framebuffer(const GLenum target);

		/*!
		 * @brief Use move constructor instead of copy constructor.
		*/
		Framebuffer(const Framebuffer& other) = delete;
		
		/*!
		 * @brief Move constructor.
		*/
		Framebuffer(Framebuffer&& other);

		/*!
		 * @brief Use move assign instead of copy assign.
		*/
		Framebuffer& operator = (const Framebuffer& other) = delete;

		/*!
		 * @brief Move assign.
		*/
		Framebuffer& operator = (Framebuffer&& other);
		~Framebuffer();


		// // ----- GENERAL METHODS ----- // //

		/*!
		 * @brief Attach `texture` to this framebuffer
		*/
		void process_texture(Texture* texture);

		/*!
		 * @brief Attach `texture` to layer at index `layer` of this framebuffer.
		*/
		void process_texture(Texture* texture, GLint layer);

		/*!
		 * @brief Make this framebuffer 'active' with regards to OpenGL.
		*/
		void bind();

		/*!
		 * @brief Unbind this framebuffer.
		*/
		void unbind();

		/*!
		 * @brief Run diagnostics to check if this framebuffer is valid. 
		 * If not, an appropriate message will be printed to the terminal.
		 * @return False if framebuffer not valid.
		*/
		bool check_bound_framebuffer();

		/*!
		 * @brief Initialize the buffer for all color attachments
		 * so that it can be rendered to.
		 * @param amount The number of buffers to draw.
		 * @param offset The number of bytes to skip before sending data (for each buffer).
		*/
		void init_color_attachments(GLuint amount, GLuint offset);
	private:
		void init_color_attachments_for_bound_framebuffer(GLuint amount, GLuint offset);
	public:
		/*!
		 * @brief Send all color output to the main window.
		 * @param dimensions Dimensions of the window.
		 * @param loc The index of the texture.
		*/
		void blit_color_to_default(const glm::uvec2& dimensions, const size_t loc);

		/*!
		 * @brief Send the stencil buffer output to the main window.
		 * @param dimensions Dimensions of the window.
		*/
		void blit_stencil_to_default(const glm::uvec2& dimensions);

		/*!
		 * @brief Send the depth buffer output to the main window.
		 * @param dimensions Dimensions of the window.
		*/
		void blit_depth_to_default(const glm::uvec2& dimensions);

	private:
		void blit(const Texture* texture, GLuint destination_id, const glm::uvec2& dimensions, const GLbitfield bitfield_mask);
		void attach_texture_to_bound_framebuffer(const Texture* texture, const GLenum attachment);
		void attach_single_texture(const Texture* texture, const GLenum attachment);

		// // ----- GETTERS ----- // //
	public:
		/*!
		 * @return The GL Framebuffer ID.
		*/
		const GLuint id() const;

		/*!
		 * @return The GL Target.
		*/

		const GLenum target() const;

		/*!
		 * @return The 'type' of framebuffer attachment (e.g. GL_DEPTH_ATTACHMENT).
		*/
		const GLenum attachment() const;

		/*!
		 * @return The mipmap level.
		*/
		const GLint level() const;

		/*!
		 * @return The number of color textures associated with this framebuffer.
		*/
		const GLuint num_color_textures() const;

		/*!
		 * @return The color texture at index `loc`.
		 * @throws Out of bounds exception if `loc` is not valid.
		*/
		const Texture* color_texture_at(const size_t loc);

		/*!
		 * @return The depth texture assicated with this framebuffer (or NULL if it doesn't exist).
		*/
		const Texture* depth_texture();

		/*!
		 * @return The stencil texture associated with this framebuffer (or NULL if it doesn't exist)
		*/
		const Texture* stencil_texture();

		/*!
		 * @return All color textures associated with this framebuffer.
		*/
		std::vector<const Texture*> color_textures();

		// // ----- SETTERS ----- // //
	public:

		/*!
		 * @brief Set the 'type' of the framebuffer attachment (eg GL_DEPTH_ATTACHMENT).
		*/
		void set_attachment(GLenum attachent);

		/*!
		 * @brief Append `textures` to the end of the existing list of textures.
		*/
		void push_back_color_buffer_textures(const std::vector<const Texture*> textures);

		/*!
		 * @brief Append `texture` to the end of the existing list of textures.
		 * @param texture 
		*/
		void push_back_color_buffer_texture(const Texture* texture);

		/*!
		 * @brief Replace the existing texture at index `loc` with `texture`.
		*/
		void set_color_buffer_texture(const Texture* texture, const GLuint loc);

		/*!
		 * @brief Replace the first texture with `texture`.
		*/
		void set_color_buffer_texture(const Texture* texture);

		/*!
		 * @brief Replace the existing depth texture with `texture`.
		*/
		void set_depth_buffer_texture(const Texture* texture);

		/*!
		 * @brief Replace the existing stencil texture with `texture`.
		*/
		void set_stencil_buffer_texture(const Texture* texture);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		GLuint m_id = 0;
		GLenum m_target = GL_FRAMEBUFFER;
		GLenum m_attachment = GL_DEPTH_ATTACHMENT;
		GLint m_level = 0;
		std::vector<const Texture*> m_color_textures;
		const Texture* m_depth_texture = NULL;
		const Texture* m_stencil_texture = NULL;
	};

}


#endif