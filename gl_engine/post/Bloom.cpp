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
		m_material.set_texture("image", &m_pingpong_textures[0]);

	}

	// // ----- GENERAL METHODS ----- // //
	void Bloom::draw()
	{
		//m_material.set_uniform("is_horizontal", true);
		//glViewport(0, 0, 800, 600);

		//m_material.update_texture_id("image", m_tone_map->bright()->id());
		//m_mesh_node.draw();
		gaussian_blur();
	}

	void Bloom::gaussian_blur()
	{
		//bool is_horizontal = true, first_iteration = true;
		//GLuint current_texture_id = m_tone_map->bright()->id();
		////GLuint texture_id = bright_id;

		////m_material.use();
		//for (int i = 0; i < PASSES; ++i)
		//{
		//	m_pingpong_fbos[is_horizontal].bind();
		//	m_material.set_uniform("is_horizontal", is_horizontal);
		//	m_material.update_texture_id("image", current_texture_id);
		//	m_mesh_node.draw();
		////	glBindTexture(GL_TEXTURE_2D, texture_id);
		////	m_material.set_texture("image", )
		////	m_mesh_node.draw();
		//	current_texture_id = (GLuint)is_horizontal;
		//	is_horizontal = !is_horizontal;

		////	//if (first_iteration)
		////	//{
		////	//	first_iteration = false;
		////	//}
		//}
		//m_pingpong_fbos[0].unbind();
		//m_mesh_node.draw();



		bool horizontal = true, first_iteration = true;
		int amount = 10;
		m_material.use();
		for (GLuint i = 0; i < amount; ++i)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_pingpong_fbos[horizontal].id());
			m_material.set_uniform("is_horizontal", horizontal);
			glBindTexture(
				GL_TEXTURE_2D, first_iteration ? m_tone_map->bright()->id() : m_pingpong_textures[!horizontal].id()
			);
			m_mesh_node.draw();
			horizontal = !horizontal;
			if (first_iteration)
			{
				first_iteration = false;
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_mesh_node.draw();
	}
}