#ifndef GL_ENGINE_SHADING_FRAMEBUFFER_H
#define GL_ENGINE_SHADING_FRAMEBUFFER_H

#include <vector>

#include <GL/glew.h>

namespace gl_engine
{
	// // ----- FORWARD DECLERATION ----- // //
	struct Texture;

	struct Framebuffer
	{
	public:
		// // ----- CONSTRUCTORS ----- // //
		Framebuffer();
		Framebuffer(const GLenum target);

		// // ----- GENERAL METHODS ----- // //
		void process_texture(Texture* texture);
		void process_texture(Texture* texture, GLint layer);
		void bind();
		void unbind();
		bool check_bound_framebuffer();
		void init_color_attachments(GLuint amount, GLuint offset);
		void init_color_attachments_for_bound_framebuffer(GLuint amount, GLuint offset);
		void add_color_buffer_textures(const std::vector<Texture*> textures);



		// // ----- GETTERS ----- // //
		GLuint id();

		// // ----- SETTERS ----- // //
		void set_attachment(GLenum attachent);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		GLuint m_id;
		GLenum m_target = GL_FRAMEBUFFER;
		GLenum m_attachment = GL_DEPTH_ATTACHMENT;
		GLint m_level = 0;
		GLuint m_num_color_buffers = 0;
		GLuint m_num_depth_buffers = 0;
		GLuint m_num_stencil_buffers = 0;
	};

} // namespace gl_engine


#endif