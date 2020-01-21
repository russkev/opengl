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
		//PostEffect::backbuffer_fbo()->init_color_attachments(2, 0);
		//PostEffect::backbuffer_fbo()->bind();

		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_beauty.id(), 0);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_bright.id(), 0);

		//PostEffect::backbuffer_fbo()->check_bound_framebuffer();
		//PostEffect::backbuffer_fbo()->unbind();

		backbuffer_fbo()->add_color_buffer_textures(std::vector<Texture*>{&m_beauty, &m_bright});


		m_material.set_texture("hdr_image", &m_beauty);
		m_material.set_uniform("exposure", 2.0f);

		/*
		
		Test
		
		*/
		//Framebuffer temp_framebuffer{ GL_FRAMEBUFFER };

		//Texture temp_texture_1 = Texture::create_color_backbuffer(GL_TEXTURE_2D, dimensions);
		//Texture temp_texture_2 = Texture::create_color_backbuffer(GL_TEXTURE_2D, dimensions);
		//std::vector<Texture*> temp_vector{ &m_beauty, &m_bright,  &temp_texture_1, &temp_texture_2 };
		//temp_framebuffer.add_color_buffer_textures(temp_vector);

		//auto sdfs = 2038;
		/*
		
		End test
		
		*/
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
