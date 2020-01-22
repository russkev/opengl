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
		backbuffer_fbo()->push_back_color_buffer_textures(std::vector<const Texture*>{&m_beauty, &m_bright});

		m_material.set_texture("hdr_image", &m_beauty);
		m_material.set_uniform("exposure", 2.0f);
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
