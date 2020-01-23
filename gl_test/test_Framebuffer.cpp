#include "pch.h"
#include <boost/test/unit_test.hpp>

#include "Helper.h"

#include "../gl_engine/shading/Framebuffer.h"

#include "../gl_engine/shading/Texture.h"

#include "../gl_engine/render/Window.h"

#include <glm/glm.hpp>
#include <GL/glew.h>

struct FramebufferTestContext
{
	FramebufferTestContext()
	{

	}
	
	gl_engine::Window window{ Helper::create_gl_window() };
	const glm::uvec2 dimensions{ 2048, 2048 };
	gl_engine::Framebuffer F1{ GL_FRAMEBUFFER };
	gl_engine::Texture color_buffer_1{ gl_engine::Texture::create_color_backbuffer(GL_TEXTURE_2D, &dimensions) };
	gl_engine::Texture color_buffer_2{ gl_engine::Texture::create_color_backbuffer(GL_TEXTURE_2D, &dimensions) };
	gl_engine::Texture color_buffer_3{ gl_engine::Texture::create_color_backbuffer(GL_TEXTURE_2D, &dimensions) };
	gl_engine::Texture color_depth_1{ gl_engine::Texture::create_color_backbuffer(GL_DEPTH, &dimensions) };
	gl_engine::Texture color_depth_2{ gl_engine::Texture::create_color_backbuffer(GL_DEPTH, &dimensions) };
	gl_engine::Texture color_stencil_1{ gl_engine::Texture::create_color_backbuffer(GL_STENCIL, &dimensions) };
	gl_engine::Texture color_stencil_2{ gl_engine::Texture::create_color_backbuffer(GL_STENCIL, &dimensions) };

};

//----------------------------------------------------------------------------------------------------//

BOOST_FIXTURE_TEST_SUITE(Framebuffer_tests, FramebufferTestContext)

BOOST_AUTO_TEST_SUITE(Texture_tests)

BOOST_AUTO_TEST_CASE(F1_set_color_1)
{
	F1.set_color_buffer_texture(&color_buffer_1, 0);
	BOOST_TEST(F1.num_color_textures() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()