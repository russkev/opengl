#include "FrameBuffer.h"

#include <stdio.h>
#include <vector>

#include "Texture.h"

namespace gl_engine_exe
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

	void Framebuffer::blit_color_to_default(const glm::uvec2& dimensions, const size_t loc)
	{
		blit(color_texture_at(loc), 0, dimensions, GL_COLOR_BUFFER_BIT);
	}

	void Framebuffer::blit_depth_to_default(const glm::uvec2& dimensions)
	{
		blit(m_depth_texture, 0, dimensions, GL_DEPTH_BUFFER_BIT);
	}

	void Framebuffer::blit_stencil_to_default(const glm::uvec2& dimensions)
	{
		blit(m_stencil_texture, 0, dimensions, GL_STENCIL_BUFFER_BIT);
	}

	void Framebuffer::blit(const Texture* texture, GLuint destination_id, const glm::uvec2& dimensions, const GLbitfield bitfield_mask)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination_id);

		glBlitFramebuffer(
			0, 0, (GLuint)(texture->width()), texture->height(), 0, 0, dimensions.x, dimensions.y, bitfield_mask, texture->min_filter());

		unbind();
	}


	void Framebuffer::push_back_color_buffer_texture(const Texture* texture)
	{
		push_back_color_buffer_textures(std::vector<const Texture*>{ texture });
	}

	void Framebuffer::push_back_color_buffer_textures(const std::vector<const Texture*> textures)
	{
		bind();
		GLuint offset = num_color_textures();
		init_color_attachments_for_bound_framebuffer((GLuint)textures.size(), num_color_textures());
		//num_color_textures() += (GLuint)textures.size();

		for (int i = 0; i < textures.size(); ++i)
		{
			attach_texture_to_bound_framebuffer(textures.at(i), GL_COLOR_ATTACHMENT0 + offset);
			m_color_textures.push_back(textures.at(i));
			offset++;
		}
		check_bound_framebuffer();
		unbind();
	}

	void Framebuffer::set_color_buffer_texture(const Texture* texture, const GLuint loc)
	{
		if (loc != 0 && loc > num_color_textures())
		{
			printf("WARNING: Unable to set framebuffer texture, location %d is not available to set", loc);
			return;
		}
		bind();
		attach_texture_to_bound_framebuffer(texture, GL_COLOR_ATTACHMENT0 + loc);
		if (m_color_textures.size() == 0)
		{
			m_color_textures.push_back(texture);
		}
		else
		{
			m_color_textures.at(loc) = texture;
		}
		unbind();
	}

	void Framebuffer::set_depth_buffer_texture(const Texture* texture)
	{
		attach_single_texture(texture, GL_DEPTH_ATTACHMENT);
		m_depth_texture = texture;
	}

	void Framebuffer::set_stencil_buffer_texture(const Texture* texture)
	{
		attach_single_texture(texture, GL_STENCIL_ATTACHMENT);
		m_stencil_texture = texture;
	}

	void Framebuffer::attach_single_texture(const Texture* texture, const GLenum attachment)
	{
		bind();
		attach_texture_to_bound_framebuffer(texture, attachment);
		check_bound_framebuffer();
		unbind();
	}

	void Framebuffer::attach_texture_to_bound_framebuffer(const Texture* texture, const GLenum attachment)
	{
		glFramebufferTexture2D(m_target, attachment, texture->target(), texture->id(), 0);
	}


	// // ----- GETTERS ----- // //
	GLuint Framebuffer::id()
	{
		return m_id;
	}

	GLuint Framebuffer::num_color_textures()
	{
		return (GLuint)m_color_textures.size();
	}

	const Texture* Framebuffer::color_texture_at(const size_t loc)
	{
		return m_color_textures.at(loc);
	}

	const Texture* Framebuffer::depth_texture()
	{
		return m_depth_texture;
	}

	const Texture* Framebuffer::stencil_texture()
	{
		return m_stencil_texture;
	}

	// // ----- SETTERS ----- // //
	void Framebuffer::set_attachment(GLenum attachment)
	{
		m_attachment = attachment;
	}
} // namespace gl_engine