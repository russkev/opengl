#include "pch.h"
#include <boost/test/unit_test.hpp>

#include <vector>

#include <glm/glm.hpp>

#include "render/DeferredRender.h"

#include "render/Window.h"
#include "material/Material.h"
#include "material/MaterialLibrary.h"

struct DeferredRenderTestContext
{
	glm::uvec2 dimensions{ 800u, 600u };
	GLenum target{ GL_TEXTURE_2D };
	glen::Window window{ "Test Window", dimensions.x, dimensions.y };
};

BOOST_FIXTURE_TEST_SUITE(DeferredRender, DeferredRenderTestContext)

BOOST_AUTO_TEST_CASE(Destructor)
{
	glen::Material* material_pointer = NULL;
	glen::Mesh* mesh_pointer = NULL;

	{
		glen::DeferredRender deferred_render(target, dimensions);
		glen::Texture g_position{ glen::Texture::create_16bit_rgb_null_texture(target, dimensions) };
		glen::Texture g_normal{ glen::Texture::create_16bit_rgb_null_texture(target, dimensions) };
		glen::Texture g_color_spec{ glen::Texture::create_8bit_rgba_null_texture(target, dimensions) };
		glen::Texture g_depth{ glen::Texture::create_depth_null_texture(target, dimensions) };

		deferred_render.framebuffer()->push_back_color_buffer_textures(std::vector<const glen::Texture*>{
			&g_position, &g_normal, &g_color_spec	});
		deferred_render.framebuffer()->set_depth_buffer_texture(&g_depth);

		deferred_render.material()->set_texture(glen::BlinnDeferredMaterial::k_g_position, &g_position);
		deferred_render.material()->set_texture(glen::BlinnDeferredMaterial::k_g_normal, &g_normal);
		deferred_render.material()->set_texture(glen::BlinnDeferredMaterial::k_g_diffuse_spec, &g_color_spec);

		material_pointer = deferred_render.material();
		mesh_pointer = deferred_render.mesh();

		BOOST_CHECK(material_pointer->program_id() != 0);
		BOOST_CHECK(mesh_pointer->num_indices() != 0);
	}

	BOOST_CHECK(material_pointer->program_id() == 0);
	BOOST_CHECK(mesh_pointer->num_indices() == 0);
}

BOOST_AUTO_TEST_CASE(Move_Constructor)
{
	glen::DeferredRender old_deferred_render(target, dimensions);
	glen::Texture g_position{ glen::Texture::create_16bit_rgb_null_texture(target, dimensions) };
	glen::Texture g_normal{ glen::Texture::create_16bit_rgb_null_texture(target, dimensions) };
	glen::Texture g_color_spec{ glen::Texture::create_8bit_rgba_null_texture(target, dimensions) };
	glen::Texture g_depth{ glen::Texture::create_depth_null_texture(target, dimensions) };

	old_deferred_render.framebuffer()->push_back_color_buffer_textures(std::vector<const glen::Texture*>{
		&g_position, &g_normal, &g_color_spec	});
	old_deferred_render.framebuffer()->set_depth_buffer_texture(&g_depth);

	old_deferred_render.material()->set_texture(glen::BlinnDeferredMaterial::k_g_position, &g_position);
	old_deferred_render.material()->set_texture(glen::BlinnDeferredMaterial::k_g_normal, &g_normal);
	old_deferred_render.material()->set_texture(glen::BlinnDeferredMaterial::k_g_diffuse_spec, &g_color_spec);

	GLuint old_FBO_id = old_deferred_render.framebuffer()->id();
	GLuint old_material_id = old_deferred_render.material()->program_id();

	BOOST_CHECK(old_FBO_id != 0);
	BOOST_CHECK(old_material_id != 0);

	glen::DeferredRender new_deferred_render{ std::move(old_deferred_render) };

	BOOST_CHECK(new_deferred_render.framebuffer()->id() == old_FBO_id);
	BOOST_CHECK(new_deferred_render.material()->program_id() == old_material_id);
	BOOST_CHECK(new_deferred_render.mesh_node()->material() == new_deferred_render.mesh_node()->material());
	BOOST_CHECK(new_deferred_render.mesh_node()->mesh() == new_deferred_render.mesh());
	//BOOST_CHECK(new_deferred_render.framebuffer()->color_texture_at(0) == new_deferred_render.texture(glen::BlinnDeferredMaterial::k_g_position));
	BOOST_CHECK(new_deferred_render.framebuffer()->depth_texture() == new_deferred_render.depth_texture());
}

BOOST_AUTO_TEST_CASE(Move_Assign)
{
	glen::DeferredRender old_deferred_render(target, dimensions);
	glen::Texture g_position{ glen::Texture::create_16bit_rgb_null_texture(target, dimensions) };
	glen::Texture g_normal{ glen::Texture::create_16bit_rgb_null_texture(target, dimensions) };
	glen::Texture g_color_spec{ glen::Texture::create_8bit_rgba_null_texture(target, dimensions) };
	glen::Texture g_depth{ glen::Texture::create_depth_null_texture(target, dimensions) };

	old_deferred_render.framebuffer()->push_back_color_buffer_textures(std::vector<const glen::Texture*>{
		&g_position, &g_normal, &g_color_spec	});
	old_deferred_render.framebuffer()->set_depth_buffer_texture(&g_depth);

	old_deferred_render.material()->set_texture(glen::BlinnDeferredMaterial::k_g_position, &g_position);
	old_deferred_render.material()->set_texture(glen::BlinnDeferredMaterial::k_g_normal, &g_normal);
	old_deferred_render.material()->set_texture(glen::BlinnDeferredMaterial::k_g_diffuse_spec, &g_color_spec);

	GLuint old_FBO_id = old_deferred_render.framebuffer()->id();
	GLuint old_material_id = old_deferred_render.material()->program_id();

	BOOST_CHECK(old_FBO_id != 0);
	BOOST_CHECK(old_material_id != 0);

	glen::DeferredRender new_deferred_render = std::move(old_deferred_render);

	BOOST_CHECK(new_deferred_render.framebuffer()->id() == old_FBO_id);
	BOOST_CHECK(new_deferred_render.material()->program_id() == old_material_id);
	BOOST_CHECK(new_deferred_render.mesh_node()->material() == new_deferred_render.mesh_node()->material());
	BOOST_CHECK(new_deferred_render.mesh_node()->mesh() == new_deferred_render.mesh());
}

//BOOST_AUTO_TEST_CASE(Factory)
//{
//	glm::uvec2 dimensions{ 800u, 600u };
//	glen::DeferredRender deferred_render = glen::DeferredRender::create_blinn_deferred(GL_TEXTURE_2D, dimensions);
//	BOOST_CHECK(deferred_render.framebuffer()->id() != 0);
//	BOOST_CHECK(deferred_render.material()->program_id() != 0);
//	BOOST_CHECK(deferred_render.dimensions() == dimensions);
//	BOOST_CHECK(deferred_render.mesh()->num_vertices() != 0);
//	BOOST_CHECK(deferred_render.mesh_node()->material() == deferred_render.material());
//	BOOST_CHECK(deferred_render.mesh_node()->mesh() == deferred_render.mesh());
//}

BOOST_AUTO_TEST_SUITE_END()