#include "pch.h"
#include "CppUnitTest.h"
#include "post/Deferred.h"

#include "render/Window.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace gl_test
{

	
	TEST_CLASS(Framebuffer)
	{
		glen::Window window{ "Window", 1u, 1u };
		const glm::uvec2 dimensions{ 128, 128 };
		glen::Framebuffer F1{ GL_FRAMEBUFFER };
		glen::Texture color_buffer_1{ glen::Texture::create_16bit_rgba_null_texture(GL_TEXTURE_2D, dimensions) };
		glen::Texture color_buffer_2{ glen::Texture::create_16bit_rgba_null_texture(GL_TEXTURE_2D, dimensions) };
		glen::Texture color_buffer_3{ glen::Texture::create_16bit_rgba_null_texture(GL_TEXTURE_2D, dimensions) };
		glen::Texture depth_buffer_1{ glen::Texture::create_depth_null_texture(GL_TEXTURE_2D, dimensions) };
		glen::Texture depth_buffer_2{ glen::Texture::create_depth_null_texture(GL_TEXTURE_2D, dimensions) };
		glen::Texture stencil_buffer_1{ glen::Texture::create_stencil_texture(GL_TEXTURE_2D, dimensions) };
		glen::Texture stencil_buffer_2{ glen::Texture::create_stencil_texture(GL_TEXTURE_2D, dimensions) };


		void add_all_textures_to_buffer()
		{
			F1.push_back_color_buffer_textures({ &color_buffer_1, &color_buffer_2, &color_buffer_3 });
			F1.set_depth_buffer_texture(&depth_buffer_1);
			F1.set_stencil_buffer_texture(&stencil_buffer_1);
		}

	public:
		TEST_METHOD_CLEANUP(Framebuffer_Cleanup)
		{
			window.~Window();
		}
		TEST_METHOD(CheckTexture_ID_Set)
		{
			Assert::IsTrue(F1.id() != 0);
		}

		TEST_METHOD(Bind)
		{
			F1.bind();
			GLint current_bound = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_bound);
			Assert::IsTrue(current_bound == F1.id());
			F1.unbind();
		}
		TEST_METHOD(Destructor)
		{
			GLint id = 0;
			GLenum target = GL_FRAMEBUFFER;

			{
				glen::Framebuffer F2{ target };
				F2.set_color_buffer_texture(&color_buffer_1);

				id = F2.id();

				auto status = glCheckNamedFramebufferStatus(id, target);
				Assert::IsTrue(status == GL_FRAMEBUFFER_COMPLETE);
			}

			auto status = glCheckNamedFramebufferStatus(id, target);

			glBindFramebuffer(target, id);

			F1.check_bound_framebuffer();

			Assert::IsTrue(status == 0);
		}

		TEST_METHOD(Move_Constructor)
		{
			add_all_textures_to_buffer();

			auto old_id = F1.id();
			auto old_target = F1.target();

			glen::Framebuffer F2{ std::move(F1) };

			Assert::IsTrue(F2.id() == old_id);
			Assert::IsTrue(F1.id() == 0);
			Assert::IsTrue(F2.target() == old_target);

			Assert::IsTrue(F2.color_texture_at(0) == &color_buffer_1);
			Assert::IsTrue(F2.color_texture_at(1) == &color_buffer_2);
			Assert::IsTrue(F2.color_texture_at(2) == &color_buffer_3);
			Assert::IsTrue(F2.depth_texture() == &depth_buffer_1);
			Assert::IsTrue(F2.stencil_texture() == &stencil_buffer_1);

			Assert::IsTrue(F1.num_color_textures() == 0);
			Assert::IsTrue(F1.depth_texture() == NULL);
			Assert::IsTrue(F1.stencil_texture() == NULL);

			F2.bind();
			GLint current_bound = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_bound);
			Assert::IsTrue(current_bound == old_id);
			F2.unbind();
		}

		TEST_METHOD(Move_Assign)
		{
			add_all_textures_to_buffer();

			auto old_id = F1.id();
			auto old_target = F1.target();

			glen::Framebuffer F2 = std::move(F1);

			Assert::IsTrue(F2.id() == old_id);
			Assert::IsTrue(F1.id() == 0);
			Assert::IsTrue(F2.target() == old_target);

			Assert::IsTrue(F2.color_texture_at(0) == &color_buffer_1);
			Assert::IsTrue(F2.color_texture_at(1) == &color_buffer_2);
			Assert::IsTrue(F2.color_texture_at(2) == &color_buffer_3);
			Assert::IsTrue(F2.depth_texture() == &depth_buffer_1);
			Assert::IsTrue(F2.stencil_texture() == &stencil_buffer_1);

			Assert::IsTrue(F1.num_color_textures() == 0);
			Assert::IsTrue(F1.depth_texture() == NULL);
			Assert::IsTrue(F1.stencil_texture() == NULL);

			F2.bind();
			GLint current_bound = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_bound);
			Assert::IsTrue(current_bound == old_id);
			F2.unbind();
		}

		TEST_METHOD(Color_1_Set)
		{
			F1.set_color_buffer_texture(&color_buffer_1, 0);
			Assert::IsTrue(F1.num_color_textures() == 1);
		}

		TEST_METHOD(Color_1_Set_Invalid)
		{
			F1.set_color_buffer_texture(&color_buffer_1, (GLuint)932879865892);
			Assert::IsTrue(F1.num_color_textures() == 0);
		}

		TEST_METHOD(Color_1_Overwrite)
		{
			F1.set_color_buffer_texture(&color_buffer_1, 0);
			F1.set_color_buffer_texture(&color_buffer_2, 0);
			Assert::IsTrue(F1.num_color_textures() == 1);
			Assert::IsTrue(F1.color_texture_at(0) == &color_buffer_2);
		}

		TEST_METHOD(Color_Multiple_Set)
		{
			std::vector<const glen::Texture*> colors{ &color_buffer_2, &color_buffer_3 };
			F1.set_color_buffer_texture(&color_buffer_1, 0);
			F1.push_back_color_buffer_textures(colors);
			Assert::IsTrue(F1.num_color_textures() == 3);
			Assert::IsTrue(F1.color_texture_at(0) == &color_buffer_1);
			Assert::IsTrue(F1.color_texture_at(1) == &color_buffer_2);
			Assert::IsTrue(F1.color_texture_at(2) == &color_buffer_3);
		}

		TEST_METHOD(Depth_Overwrite)
		{
			F1.set_depth_buffer_texture(&depth_buffer_1);
			F1.set_depth_buffer_texture(&depth_buffer_2);
			Assert::IsTrue(F1.depth_texture() == &depth_buffer_2);
		}

		TEST_METHOD(Stencil_Overwrite)
		{
			F1.set_stencil_buffer_texture(&stencil_buffer_1);
			F1.set_stencil_buffer_texture(&stencil_buffer_2);
			Assert::IsTrue(F1.stencil_texture() == &stencil_buffer_2);
		}
	};
}