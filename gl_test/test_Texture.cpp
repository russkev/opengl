#include "pch.h"
#include <boost/test/unit_test.hpp>

#include "shading/Texture.h"
#include "render/Window.h"

#undef main

struct TextureTestContext
{
	static GLint check_texture_binding(const GLuint id)
	{
		glBindTexture(GL_TEXTURE_2D, id);
		GLint which_id;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &which_id);
		glBindTexture(GL_TEXTURE_2D, 0);
		return which_id;
	}

	glen::Window window{ "Test window", 800u, 600u };
};

BOOST_FIXTURE_TEST_SUITE(Texture, TextureTestContext)

BOOST_AUTO_TEST_CASE(Destructor)
{
	GLuint id = 0;
	const SDL_Surface* surface;
	{
		glen::Texture texture("uvtemplate.tga");
		id = texture.id();
		surface = texture.surface();

		GLint which_id = check_texture_binding(id);

		BOOST_CHECK(which_id == id);
		BOOST_CHECK(surface->map != NULL);
	}
	GLint which_id = check_texture_binding(id);

	BOOST_CHECK(which_id == 0);
	BOOST_CHECK(surface->map == NULL);
}

BOOST_AUTO_TEST_CASE(Move_Constructor_With_Texture)
{
	glen::Texture texture_old("uvtemplate.tga");
	const SDL_Surface* surface = texture_old.surface();
	const GLuint id = texture_old.id();

	GLint which_id = check_texture_binding(id);

	BOOST_CHECK(which_id == id);
	BOOST_CHECK(surface->map != NULL);

	glen::Texture texture_new(std::move(texture_old));
	which_id = check_texture_binding(texture_new.id());

	BOOST_CHECK(which_id == id);
	BOOST_CHECK(texture_old.id() == 0);
	BOOST_CHECK(texture_new.id() == id);
	BOOST_CHECK(texture_new.surface()->map == surface->map);
}

BOOST_AUTO_TEST_CASE(Move_Constructor_With_Color)
{
	glen::Texture texture_old(glm::vec3{ 0.2, 0.1, 0.5 });
	auto texture_old_color = texture_old.color();
	glen::Texture texture_new(std::move(texture_old));

	BOOST_CHECK(texture_new.color() == texture_old_color);
}

BOOST_AUTO_TEST_CASE(Move_Assign_With_Texture)
{
	glen::Texture texture_old("uvtemplate.tga");
	glen::Texture texture_new{ GL_TEXTURE_2D };
	const SDL_Surface* surface = texture_old.surface();
	const GLuint id = texture_old.id();

	GLint which_id = check_texture_binding(id);

	BOOST_CHECK(which_id == id);
	BOOST_CHECK(surface->map != NULL);

	texture_new = std::move(texture_old);
	which_id = check_texture_binding(texture_new.id());

	BOOST_CHECK(which_id == id);
	BOOST_CHECK(texture_old.id() == 0);
	BOOST_CHECK(texture_new.id() == id);
	BOOST_CHECK(texture_new.surface()->map == surface->map);
}

BOOST_AUTO_TEST_CASE(Move_Assign_With_Color)
{
	glen::Texture texture_old(glm::vec3{ 0.2, 0.1, 0.5 });
	glen::Texture texture_new(GL_TEXTURE_2D);
	auto texture_old_color = texture_old.color();
	texture_new = std::move(texture_old);

	BOOST_CHECK(texture_new.color() == texture_old_color);
}

BOOST_AUTO_TEST_SUITE_END()