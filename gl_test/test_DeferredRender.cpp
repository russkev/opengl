#include "pch.h"
#include <boost/test/unit_test.hpp>

#include "render/DeferredRender.h"

#include "render/Window.h"

struct DeferredRenderTestContext
{
	glen::Window window{ "Test Window", 800u, 600u };
};

BOOST_FIXTURE_TEST_SUITE(DeferredRender, DeferredRenderTestContext)

BOOST_AUTO_TEST_CASE(Factory)
{
	glm::uvec2 dimensions{ 800u, 600u };
	glen::DeferredRender deferred_render = glen::DeferredRender::create_blinn_deferred(GL_TEXTURE_2D, dimensions);
}

BOOST_AUTO_TEST_SUITE_END()