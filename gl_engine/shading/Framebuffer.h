#ifndef GL_ENGINE_SHADING_FRAMEBUFFER_H
#define GL_ENGINE_SHADING_FRAMEBUFFER_H

#include <vector>
#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace glen
{
	// // ----- FORWARD DECLERATION ----- // //
	struct Texture;

	struct Framebuffer
	{
	public:
		// // ----- CONSTRUCTORS ----- // //
		Framebuffer();
		Framebuffer(const GLenum target);
		//~Framebuffer();

		// // ----- GENERAL METHODS ----- // //
		void process_texture(Texture* texture);
		void process_texture(Texture* texture, GLint layer);
		void bind();
		void unbind();
		bool check_bound_framebuffer();
		void init_color_attachments(GLuint amount, GLuint offset);
		void init_color_attachments_for_bound_framebuffer(GLuint amount, GLuint offset);
		void blit_color_to_default(const glm::uvec2& dimensions, const size_t loc);
		void blit_stencil_to_default(const glm::uvec2& dimensions);
		void blit_depth_to_default(const glm::uvec2& dimensions);

	private:
		void blit(const Texture* texture, GLuint destination_id, const glm::uvec2& dimensions, const GLbitfield bitfield_mask);
		void attach_texture_to_bound_framebuffer(const Texture* texture, const GLenum attachment);
		void attach_single_texture(const Texture* texture, const GLenum attachment);

		// // ----- GETTERS ----- // //
	public:
		GLuint id();
		GLuint num_color_textures();
		const Texture* color_texture_at(const size_t loc);
		const Texture* depth_texture();
		const Texture* stencil_texture();

		// // ----- SETTERS ----- // //
	public:
		void set_attachment(GLenum attachent);
		void push_back_color_buffer_textures(const std::vector<const Texture*> textures);
		void push_back_color_buffer_texture(const Texture* texture);
		void set_color_buffer_texture(const Texture* texture, const GLuint loc);
		void set_depth_buffer_texture(const Texture* texture);
		void set_stencil_buffer_texture(const Texture* texture);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		GLuint m_id = 0;
		GLenum m_target = GL_FRAMEBUFFER;
		GLenum m_attachment = GL_DEPTH_ATTACHMENT;
		GLint m_level = 0;
		//GLuint m_num_color_buffers = 0;
		std::vector<const Texture*> m_color_textures;
		const Texture* m_depth_texture = NULL;
		const Texture* m_stencil_texture = NULL;
	};

}


#endif