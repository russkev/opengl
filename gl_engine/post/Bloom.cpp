#include "Bloom.h"

#include "ToneMap.h"

#include "../shading/Framebuffer.h"


namespace gl_engine
{
	// // ----- CONSTRUCTOR ----- // //
	Bloom::Bloom(Framebuffer* backbuffer, const glm::uvec2* dimensions, ToneMap* tone_map) :
		PostEffect::PostEffect(backbuffer),
		m_tone_map{tone_map}
	{
		for (GLuint i = 0; i < DIRECTIONS; ++i)
		{
			m_pingpong_textures[i] = Texture::create_color_backbuffer(GL_TEXTURE_2D, dimensions);
			m_pingpong_fbos[i].bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingpong_textures[i].id(), 0);
			m_pingpong_fbos[i].check_bound_framebuffer();
			m_pingpong_fbos[i].unbind();
		}

		m_blur_texture = Texture::create_color_backbuffer(GL_TEXTURE_2D, dimensions);
		//m_blur_material.set_texture("image", m_tone_map->bright());
		m_blur_material.set_texture("image", &m_blur_texture);
		m_bloom_material.set_texture("color", m_tone_map->beauty());
		m_bloom_material.set_texture("bright", &m_blur_texture);
	}

	// // ----- GENERAL METHODS ----- // //
	void Bloom::draw()
	{
		gaussian_blur();
	}

	void Bloom::gaussian_blur()
	{
		bool horizontal = true;
		GLuint amount = 20;
		GLuint texture_id = m_tone_map->bright()->id();

		m_blur_material.use();
		for (GLuint i = 0; i < amount; ++i)
		{
			m_pingpong_fbos[horizontal].bind();
			m_blur_material.set_uniform("is_horizontal", horizontal);
			m_blur_texture.set_new_id(texture_id);
			m_blur_node.draw();
			texture_id = m_pingpong_textures[horizontal].id();
			horizontal = !horizontal;
		}
		m_pingpong_fbos[horizontal].unbind();

		combine_blur();
	}

	void Bloom::combine_blur()
	{
		m_bloom_node.draw();
	}
}
