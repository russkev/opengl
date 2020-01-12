#ifndef GL_ENGINE_SHADING_FRAMEBUFFER_H
#define GL_ENGINE_SHADING_FRAMEBUFFER_H

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


		// // ----- SETTERS ----- // //
		void set_attachment(GLenum attachent);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		GLuint m_id;
		GLenum m_target = GL_FRAMEBUFFER;
		GLenum m_attachment = GL_DEPTH_ATTACHMENT;
		GLint m_level = 0;
	};

} // namespace gl_engine


#endif