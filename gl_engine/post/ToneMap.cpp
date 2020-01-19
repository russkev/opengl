#include "ToneMap.h"

#include "../shading/Framebuffer.h"

namespace gl_engine
{
	// // ----- CONSTRUCTOR ----- // //
	ToneMap::ToneMap(Framebuffer* backbuffer, const glm::uvec2* dimensions) :
		PostEffect::PostEffect(backbuffer),
		m_beauty{ Texture::create_color_backbuffer(GL_TEXTURE_2D, dimensions) },
		m_bright{ Texture::create_color_backbuffer(GL_TEXTURE_2D, dimensions) }
	{
		PostEffect::backbuffer_fbo()->init_color_attachments(2, 0);
		PostEffect::backbuffer_fbo()->bind();

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_beauty.id(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_bright.id(), 0);

		PostEffect::backbuffer_fbo()->check_bound_framebuffer();
		PostEffect::backbuffer_fbo()->unbind();

		m_material.set_texture("hdr_buffer", &m_beauty);
		m_material.set_uniform("exposure", 1.0f);
	}

	// // ----- GENERAL METHODS ----- // //
	void ToneMap::draw()
	{
		m_mesh_node.draw();
	}

	// // ----- GETTERS ----- // //
	Texture* ToneMap::beauty()
	{
		return &m_beauty;
	}

	Texture* ToneMap::bright()
	{
		return &m_bright;
	}

}
