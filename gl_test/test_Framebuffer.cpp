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
	FramebufferTestContext(){}
	
	glen::Window window{ Helper::create_gl_window() };
	const glm::uvec2 dimensions{ 2048, 2048 };
	glen::Framebuffer F1{ GL_FRAMEBUFFER };
	glen::Texture color_buffer_1{ glen::Texture::create_16bit_rgba_null_texture(GL_TEXTURE_2D, &dimensions) };
	glen::Texture color_buffer_2{ glen::Texture::create_16bit_rgba_null_texture(GL_TEXTURE_2D, &dimensions) };
	glen::Texture color_buffer_3{ glen::Texture::create_16bit_rgba_null_texture(GL_TEXTURE_2D, &dimensions) };
	glen::Texture depth_buffer_1{ glen::Texture::create_depth_null_texture (GL_TEXTURE_2D, &dimensions) };
	glen::Texture depth_buffer_2{ glen::Texture::create_depth_null_texture(GL_TEXTURE_2D, &dimensions) };
	glen::Texture stencil_buffer_1{ glen::Texture::create_stencil_texture(GL_TEXTURE_2D, &dimensions) };
	glen::Texture stencil_buffer_2{ glen::Texture::create_stencil_texture(GL_TEXTURE_2D, &dimensions) };

};

//----------------------------------------------------------------------------------------------------//

BOOST_FIXTURE_TEST_SUITE(Framebuffer_tests, FramebufferTestContext)

BOOST_AUTO_TEST_SUITE(binding_tests)

BOOST_AUTO_TEST_CASE(F1_check_texture_id_set)
{
	BOOST_TEST(F1.id() != 0);
}

BOOST_AUTO_TEST_CASE(F1_check_binding)
{
	F1.bind();
	GLint current_bound = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_bound);
	BOOST_TEST(current_bound == F1.id());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(buffer_texture_tests)


BOOST_AUTO_TEST_CASE(F1_set_color_1)
{
	F1.set_color_buffer_texture(&color_buffer_1, 0);
	BOOST_TEST(F1.num_color_textures() == 1);
}

BOOST_AUTO_TEST_CASE(F1_set_color_1_invalid)
{
	F1.set_color_buffer_texture(&color_buffer_1, (GLuint)932879865892);
	BOOST_TEST(F1.num_color_textures() == 0);
}

BOOST_AUTO_TEST_CASE(F1_overwrite_color_1)
{
	F1.set_color_buffer_texture(&color_buffer_1, 0);
	F1.set_color_buffer_texture(&color_buffer_2, 0);
	BOOST_TEST(F1.num_color_textures() == 1);
	BOOST_TEST(F1.color_texture_at(0) == &color_buffer_2);
}

BOOST_AUTO_TEST_CASE(F1_set_multiple_color_buffer_textures)
{
	std::vector<const glen::Texture*> colors{ &color_buffer_2, &color_buffer_3 };
	F1.set_color_buffer_texture(&color_buffer_1, 0);
	F1.push_back_color_buffer_textures(colors);
	BOOST_TEST(F1.num_color_textures() == 3);
	BOOST_TEST(F1.color_texture_at(0) == &color_buffer_1);
	BOOST_TEST(F1.color_texture_at(1) == &color_buffer_2);
	BOOST_TEST(F1.color_texture_at(2) == &color_buffer_3);
}

BOOST_AUTO_TEST_CASE(F1_overwrite_depth_1)
{
	F1.set_depth_buffer_texture(&depth_buffer_1);
	F1.set_depth_buffer_texture(&depth_buffer_2);
	BOOST_TEST(F1.depth_texture() == &depth_buffer_2);
}

BOOST_AUTO_TEST_CASE(F1_overwrite_stencil_1)
{
	F1.set_stencil_buffer_texture(&stencil_buffer_1);
	F1.set_stencil_buffer_texture(&stencil_buffer_2);
	BOOST_TEST(F1.stencil_texture() == &stencil_buffer_2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()