#include "pch.h"
#include "CppUnitTest.h"
#include "post/Deferred.h"

#include "render/Window.h"
#include "material/Material.h"
#include "material/MaterialLibrary.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace gl_test
{
	TEST_CLASS(Deferred)
	{
		glm::uvec2 dimensions{ 1u, 1u };
		glen::Window window{ "Test Window", dimensions.x, dimensions.y };
		GLenum target{ GL_TEXTURE_2D };
		glen::Framebuffer framebuffer{ GL_FRAMEBUFFER };
		glen::BlinnDeferredMaterial material;

	public:
		TEST_METHOD(BlinnDeferred)
		{
		

			glen::BlinnDeferred deferred_render{ GL_TEXTURE_2D, &framebuffer, dimensions };
			Assert::IsTrue(deferred_render.framebuffer()->id() != 0);
			Assert::IsTrue(deferred_render.material()->program_id() != 0);
			Assert::IsTrue(deferred_render.dimensions() == dimensions);
			Assert::IsTrue(deferred_render.mesh()->num_vertices() != 0);
			Assert::IsTrue(deferred_render.mesh_node()->material() == deferred_render.material());
			Assert::IsTrue(deferred_render.mesh_node()->mesh() == deferred_render.mesh());
			Assert::IsTrue(deferred_render.framebuffer()->depth_texture() == deferred_render.depth_texture());
		}
		TEST_METHOD(Destructor)
		{
			glen::Material* material_pointer = NULL;
			glen::Mesh* mesh_pointer = NULL;

			{
				glen::Deferred deferred(target, &framebuffer, &material, dimensions);
				glen::Texture g_position{ glen::Texture::create_16bit_rgb_null_texture(target, dimensions) };
				glen::Texture g_normal{ glen::Texture::create_16bit_rgb_null_texture(target, dimensions) };
				glen::Texture g_color_spec{ glen::Texture::create_8bit_rgba_null_texture(target, dimensions) };
				glen::Texture g_depth{ glen::Texture::create_depth_null_texture(target, dimensions) };

				deferred.framebuffer()->push_back_color_buffer_textures(std::vector<const glen::Texture*>{
					&g_position, & g_normal, & g_color_spec	});
				deferred.framebuffer()->set_depth_buffer_texture(&g_depth);

				deferred.material()->set_texture(glen::BlinnDeferredMaterial::k_g_position, &g_position);
				deferred.material()->set_texture(glen::BlinnDeferredMaterial::k_g_normal, &g_normal);
				deferred.material()->set_texture(glen::BlinnDeferredMaterial::k_g_diffuse_spec, &g_color_spec);


				material_pointer = deferred.material();
				mesh_pointer = deferred.mesh();

				Assert::IsTrue(material_pointer->program_id() != 0);
				Assert::IsTrue(mesh_pointer->num_indices() != 0);
			}

			Assert::IsTrue(mesh_pointer->num_indices() == 0);
		}

		TEST_METHOD(Move_Constructor)
		{
			glen::Deferred old_deferred_render(target, &framebuffer, &material, dimensions);

			old_deferred_render.set_depth_texture(glen::Texture::create_depth_null_texture(target, dimensions));
			old_deferred_render.set_color_texture(0,
				glen::Texture::create_16bit_rgb_null_texture(glen::BlinnDeferredMaterial::k_g_position, target, dimensions));
			old_deferred_render.send_color_textures_to_framebuffer();

			GLuint old_FBO_id = old_deferred_render.framebuffer()->id();
			GLuint old_material_id = old_deferred_render.material()->program_id();

			Assert::IsTrue(old_FBO_id != 0);
			Assert::IsTrue(old_material_id != 0);

			glen::Deferred new_deferred_render{ std::move(old_deferred_render) };

			Assert::IsTrue(new_deferred_render.framebuffer()->id() == old_FBO_id);
			Assert::IsTrue(new_deferred_render.material()->program_id() == old_material_id);
			Assert::IsTrue(new_deferred_render.mesh_node()->material() == new_deferred_render.mesh_node()->material());
			Assert::IsTrue(new_deferred_render.mesh_node()->mesh() == new_deferred_render.mesh());
			Assert::IsTrue(new_deferred_render.framebuffer()->depth_texture() == new_deferred_render.depth_texture());
			Assert::IsTrue(new_deferred_render.framebuffer()->color_texture_at(0) ==
				new_deferred_render.texture(0u));
		}

		TEST_METHOD(Move_Assign_Internal_Textures)
		{
			glen::Deferred old_deferred_render(target, &framebuffer, &material, dimensions);

			old_deferred_render.set_depth_texture(glen::Texture::create_depth_null_texture(target, dimensions));
			old_deferred_render.set_color_texture(0,
				glen::Texture::create_16bit_rgb_null_texture(glen::BlinnDeferredMaterial::k_g_position, target, dimensions));
			old_deferred_render.send_color_textures_to_framebuffer();

			GLuint old_FBO_id = old_deferred_render.framebuffer()->id();
			GLuint old_material_id = old_deferred_render.material()->program_id();

			Assert::IsTrue(old_FBO_id != 0);
			Assert::IsTrue(old_material_id != 0);

			glen::Deferred new_deferred_render = std::move(old_deferred_render);

			Assert::IsTrue(new_deferred_render.framebuffer()->id() == old_FBO_id);
			Assert::IsTrue(new_deferred_render.material()->program_id() == old_material_id);
			Assert::IsTrue(new_deferred_render.mesh_node()->material() == new_deferred_render.mesh_node()->material());
			Assert::IsTrue(new_deferred_render.mesh_node()->mesh() == new_deferred_render.mesh());
			Assert::IsTrue(new_deferred_render.framebuffer()->depth_texture() == new_deferred_render.depth_texture());
			Assert::IsTrue(new_deferred_render.framebuffer()->color_texture_at(0) ==
				new_deferred_render.texture(0u));
		}

		TEST_METHOD(Move_Assign_External_Textures)
		{

			glen::Deferred old_deferred_render(target, &framebuffer, &material, dimensions);

			glen::Texture g_position{ glen::Texture::create_16bit_rgb_null_texture(glen::BlinnDeferredMaterial::k_g_position, target, dimensions) };
			glen::Texture g_depth{ glen::Texture::create_depth_null_texture(target, dimensions) };



			old_deferred_render.set_depth_texture(&g_depth);
			old_deferred_render.set_color_texture(0u, &g_position);
			old_deferred_render.send_color_textures_to_framebuffer();

			GLuint old_FBO_id = old_deferred_render.framebuffer()->id();
			GLuint old_material_id = old_deferred_render.material()->program_id();

			Assert::IsTrue(old_FBO_id != 0);
			Assert::IsTrue(old_material_id != 0);

			glen::Deferred new_deferred_render = std::move(old_deferred_render);

			Assert::IsTrue(new_deferred_render.framebuffer()->id() == old_FBO_id);
			Assert::IsTrue(new_deferred_render.material()->program_id() == old_material_id);
			Assert::IsTrue(new_deferred_render.mesh_node()->material() == new_deferred_render.mesh_node()->material());
			Assert::IsTrue(new_deferred_render.mesh_node()->mesh() == new_deferred_render.mesh());
			Assert::IsTrue(new_deferred_render.framebuffer()->depth_texture() == &g_depth);
			Assert::IsTrue(new_deferred_render.framebuffer()->color_texture_at(0) == &g_position);
		}


	};

}