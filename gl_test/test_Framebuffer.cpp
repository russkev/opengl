#include "pch.h"
#include <boost/test/unit_test.hpp>

#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Helper.h"

#include "../gl_engine/shading/Framebuffer.h"
#include "../gl_engine/shading/Texture.h"
#include "../gl_engine/render/Window.h"



struct FramebufferTestContext
{
	FramebufferTestContext(){}
	
	glen::Window window{ "Test Window", 800u, 600u };
	const glm::uvec2 dimensions{ 2048, 2048 };
	glen::Framebuffer F1{ GL_FRAMEBUFFER };
	glen::Texture color_buffer_1{ glen::Texture::create_16bit_rgba_null_texture(GL_TEXTURE_2D, dimensions) };
	glen::Texture color_buffer_2{ glen::Texture::create_16bit_rgba_null_texture(GL_TEXTURE_2D, dimensions) };
	glen::Texture color_buffer_3{ glen::Texture::create_16bit_rgba_null_texture(GL_TEXTURE_2D, dimensions) };
	glen::Texture depth_buffer_1{ glen::Texture::create_depth_null_texture (GL_TEXTURE_2D, dimensions) };
	glen::Texture depth_buffer_2{ glen::Texture::create_depth_null_texture(GL_TEXTURE_2D, dimensions) };
	glen::Texture stencil_buffer_1{ glen::Texture::create_stencil_texture(GL_TEXTURE_2D, dimensions) };
	glen::Texture stencil_buffer_2{ glen::Texture::create_stencil_texture(GL_TEXTURE_2D, dimensions) };

	void add_all_textures_to_buffer()
	{
		F1.push_back_color_buffer_textures({&color_buffer_1, &color_buffer_2, &color_buffer_3});
		F1.set_depth_buffer_texture(&depth_buffer_1);
		F1.set_stencil_buffer_texture(&stencil_buffer_1);
	}
};

//----------------------------------------------------------------------------------------------------//

BOOST_FIXTURE_TEST_SUITE(Framebuffer, FramebufferTestContext)

BOOST_AUTO_TEST_CASE(CheckTexture_ID_Set)
{
	BOOST_TEST(F1.id() != 0);
}

BOOST_AUTO_TEST_CASE(Bind)
{
	F1.bind();
	GLint current_bound = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_bound);
	BOOST_TEST(current_bound == F1.id());
	F1.unbind();
}

BOOST_AUTO_TEST_CASE(Destructor)
{
	GLint id = 0;
	GLenum target = GL_FRAMEBUFFER;
	
	{
		glen::Framebuffer F2{ target };
		F2.set_color_buffer_texture(&color_buffer_1);

		id = F2.id();

		auto status = glCheckNamedFramebufferStatus(id, target);
		BOOST_CHECK(status == GL_FRAMEBUFFER_COMPLETE);

		//F2.~Framebuffer();
	}

	auto status  = glCheckNamedFramebufferStatus(id, target);

	glBindFramebuffer(target, id);

	F1.check_bound_framebuffer();

	BOOST_CHECK(status == 0);
}

BOOST_AUTO_TEST_CASE(Move_Constructor)
{
	add_all_textures_to_buffer();
	
	auto old_id = F1.id();
	auto old_target = F1.target();

	glen::Framebuffer F2{ std::move(F1) };

	BOOST_CHECK(F2.id() == old_id);
	BOOST_CHECK(F1.id() == 0);
	BOOST_CHECK(F2.target() == old_target);

	BOOST_CHECK(F2.color_texture_at(0) == &color_buffer_1);
	BOOST_CHECK(F2.color_texture_at(1) == &color_buffer_2);
	BOOST_CHECK(F2.color_texture_at(2) == &color_buffer_3);
	BOOST_CHECK(F2.depth_texture() == &depth_buffer_1);
	BOOST_CHECK(F2.stencil_texture() == &stencil_buffer_1);
	
	BOOST_CHECK(F1.num_color_textures() == 0);
	BOOST_CHECK(F1.depth_texture() == NULL);
	BOOST_CHECK(F1.stencil_texture() == NULL);

	F2.bind();
	GLint current_bound = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_bound);
	BOOST_CHECK(current_bound == old_id);
	F2.unbind();
}

BOOST_AUTO_TEST_CASE(Move_Assign)
{
	add_all_textures_to_buffer();

	auto old_id = F1.id();
	auto old_target = F1.target();

	glen::Framebuffer F2 = std::move(F1);

	BOOST_CHECK(F2.id() == old_id);
	BOOST_CHECK(F1.id() == 0);
	BOOST_CHECK(F2.target() == old_target);

	BOOST_CHECK(F2.color_texture_at(0) == &color_buffer_1);
	BOOST_CHECK(F2.color_texture_at(1) == &color_buffer_2);
	BOOST_CHECK(F2.color_texture_at(2) == &color_buffer_3);
	BOOST_CHECK(F2.depth_texture() == &depth_buffer_1);
	BOOST_CHECK(F2.stencil_texture() == &stencil_buffer_1);

	BOOST_CHECK(F1.num_color_textures() == 0);
	BOOST_CHECK(F1.depth_texture() == NULL);
	BOOST_CHECK(F1.stencil_texture() == NULL);

	F2.bind();
	GLint current_bound = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_bound);
	BOOST_CHECK(current_bound == old_id);
	F2.unbind();
}

BOOST_AUTO_TEST_CASE(Color_1_Set)
{
	F1.set_color_buffer_texture(&color_buffer_1, 0);
	BOOST_TEST(F1.num_color_textures() == 1);
}

BOOST_AUTO_TEST_CASE(Color_1_Set_Invalid)
{
	F1.set_color_buffer_texture(&color_buffer_1, (GLuint)932879865892);
	BOOST_TEST(F1.num_color_textures() == 0);
}

BOOST_AUTO_TEST_CASE(Color_1_Overwrite)
{
	F1.set_color_buffer_texture(&color_buffer_1, 0);
	F1.set_color_buffer_texture(&color_buffer_2, 0);
	BOOST_TEST(F1.num_color_textures() == 1);
	BOOST_TEST(F1.color_texture_at(0) == &color_buffer_2);
}

BOOST_AUTO_TEST_CASE(Color_Multiple_Set)
{
	std::vector<const glen::Texture*> colors{ &color_buffer_2, &color_buffer_3 };
	F1.set_color_buffer_texture(&color_buffer_1, 0);
	F1.push_back_color_buffer_textures(colors);
	BOOST_TEST(F1.num_color_textures() == 3);
	BOOST_TEST(F1.color_texture_at(0) == &color_buffer_1);
	BOOST_TEST(F1.color_texture_at(1) == &color_buffer_2);
	BOOST_TEST(F1.color_texture_at(2) == &color_buffer_3);
}

BOOST_AUTO_TEST_CASE(Depth_Overwrite)
{
	F1.set_depth_buffer_texture(&depth_buffer_1);
	F1.set_depth_buffer_texture(&depth_buffer_2);
	BOOST_TEST(F1.depth_texture() == &depth_buffer_2);
}

BOOST_AUTO_TEST_CASE(Stencil_Overwrite)
{
	F1.set_stencil_buffer_texture(&stencil_buffer_1);
	F1.set_stencil_buffer_texture(&stencil_buffer_2);
	BOOST_TEST(F1.stencil_texture() == &stencil_buffer_2);
}

BOOST_AUTO_TEST_SUITE_END()