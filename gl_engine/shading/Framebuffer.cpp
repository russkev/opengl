#include "FrameBuffer.h"

#include <stdio.h>
#include <vector>

#include "Texture.h"

namespace gl_engine
{
	// // ----- CONSTRUCTORS ----- // //
	Framebuffer::Framebuffer()
	{
		glGenFramebuffers(1, &m_id);
	}

	Framebuffer::Framebuffer(const GLenum target) :
		m_target{target}
	{
		glGenFramebuffers(1, &m_id);
	}


	// // ----- GENERAL METHODS ----- // //
	void Framebuffer::process_texture(Texture* texture)
	{
		GLuint id = texture->id();
		glFramebufferTexture(
			m_target,
			m_attachment,
			texture->id(),
			m_level
		);
	}

	// Multi-layer framebuffer
	void Framebuffer::process_texture(Texture* texture, GLint layer)
	{
		glFramebufferTextureLayer(
			m_target,
			m_attachment,
			texture->id(),
			m_level,
			layer
		);
	}

	void Framebuffer::bind()
	{
		glBindFramebuffer(m_target, m_id);
	}

	void Framebuffer::unbind()
	{
		glBindFramebuffer(m_target, 0);
	}

	bool Framebuffer::check_bound_framebuffer()
	{
		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("WARNING: ");
			switch (status)
			{
			case GL_FRAMEBUFFER_UNDEFINED:
				printf("FRAMEBUFFER_UNDEFINED \nDefault framebuffer bound but does not exist\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				printf("FRAMEBUFFER_INCOMPLETE_ATTACHMENT \nOne of the framebuffer attachment points is incomplete\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				printf("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT \nFramebuffer does not have at least one image attached\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				printf("FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER \nFramebuffer attachment object type is 'GL_NONE' for a color attachment point named by GL_DRAW_BUFFERi\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				printf("FRAMEBUFFER_INCOMPLETE_READ_BUFFER \nGL_READ_BUFFER is assigned and framebuffer attachment object is GL_NONE for color attachment point named by GL_READ_BUFFER\n");
				return false;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				printf("GL_FRAMEBUFFER_UNSUPPORTED \n A combination of internal formats of attached images violates implementation-dependent restrictions\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				printf("FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				printf("FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS \n A returned framebuffer is layered and a populated attachment is not layered\n");
				return false;
			}
		}
		return true;
	}

	void Framebuffer::init_color_attachments(GLuint amount, GLuint offset)
	{
		bind();
		init_color_attachments_for_bound_framebuffer(amount, offset);
		unbind();
	}

	void Framebuffer::init_color_attachments_for_bound_framebuffer(GLuint amount, GLuint offset)
	{
		std::vector<GLenum> attachments;
		for (GLuint i = 0; i < amount; ++i)
		{
			attachments.push_back(GL_COLOR_ATTACHMENT0 + offset + i);
		}
		glDrawBuffers(amount, attachments.data());
	}

	void Framebuffer::add_color_buffer_textures(const std::vector<Texture*> textures)
	{
		bind();
		GLuint offset = m_num_color_buffers;
		init_color_attachments_for_bound_framebuffer((GLuint)textures.size(), offset);
		m_num_color_buffers += (GLuint)textures.size();

		for (int i = 0; i < textures.size(); ++i)
		{
			glFramebufferTexture2D(m_target, GL_COLOR_ATTACHMENT0 + offset + i, textures.at(i)->target(), textures.at(i)->id(), 0);
		}

		check_bound_framebuffer();
		unbind();

	}


	// // ----- GETTERS ----- // //
	GLuint Framebuffer::id()
	{
		return m_id;
	}

	// // ----- SETTERS ----- // //
	void Framebuffer::set_attachment(GLenum attachment)
	{
		m_attachment = attachment;
	}
} // namespace gl_engine