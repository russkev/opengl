#include "pch.h"
#include "CppUnitTest.h"

#include "shading/Texture.h"
#include "render/Window.h"
#include "material/Material.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace gl_test
{
	TEST_CLASS(Texture)
	{
		static GLint check_texture_binding(const GLuint id)
		{
			glBindTexture(GL_TEXTURE_2D, id);
			GLint which_id;
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &which_id);
			glBindTexture(GL_TEXTURE_2D, 0);
			return which_id;
		}

		glen::Window window{ "Test window", 1u, 1u };

	public:
		TEST_METHOD(Destructor)
		{
			GLuint id = 0;
			const SDL_Surface* surface;
			{
				glen::Texture texture(
					(glen::Material::shaders_dir() + "..\\textures\\utility\\uv_template.tga").c_str());
				id = texture.id();
				surface = texture.surface();

				GLint which_id = check_texture_binding(id);

				Assert::IsTrue(which_id == id);
				Assert::IsTrue(surface->map != NULL);
			}
			GLint which_id = check_texture_binding(id);

			Assert::IsTrue(which_id == 0);
		}

		TEST_METHOD(Move_Constructor_With_Texture)
		{
			
			glen::Texture texture_old(
				(glen::Material::shaders_dir() + "..\\textures\\utility\\uv_template.tga").c_str());
			const SDL_Surface* surface = texture_old.surface();
			const GLuint id = texture_old.id();

			GLint which_id = check_texture_binding(id);

			Assert::IsTrue(which_id == id);
			Assert::IsTrue(surface->map != NULL);

			glen::Texture texture_new(std::move(texture_old));
			which_id = check_texture_binding(texture_new.id());

			Assert::IsTrue(which_id == id);
			Assert::IsTrue(texture_old.id() == 0);
			Assert::IsTrue(texture_new.id() == id);
			Assert::IsTrue(texture_new.surface()->map == surface->map);
		}

		TEST_METHOD(Move_Constructor_With_Color)
		{
			glen::Texture texture_old(glm::vec3{ 0.2, 0.1, 0.5 });
			auto texture_old_color = texture_old.color();
			glen::Texture texture_new(std::move(texture_old));

			Assert::IsTrue(texture_new.color() == texture_old_color);
		}

		TEST_METHOD(Move_Assign_With_Texture)
		{
			glen::Texture texture_old(
				(glen::Material::shaders_dir() + "..\\textures\\utility\\uv_template.tga").c_str());
			glen::Texture texture_new{ GL_TEXTURE_2D };
			const SDL_Surface* surface = texture_old.surface();
			const GLuint id = texture_old.id();

			GLint which_id = check_texture_binding(id);

			Assert::IsTrue(which_id == id);
			Assert::IsTrue(surface->map != NULL);

			texture_new = std::move(texture_old);
			which_id = check_texture_binding(texture_new.id());

			Assert::IsTrue(which_id == id);
			Assert::IsTrue(texture_old.id() == 0);
			Assert::IsTrue(texture_new.id() == id);
			Assert::IsTrue(texture_new.surface()->map == surface->map);
		}

		TEST_METHOD(Move_Assign_With_Color)
		{
			glen::Texture texture_old(glm::vec3{ 0.2, 0.1, 0.5 });
			glen::Texture texture_new(GL_TEXTURE_2D);
			auto texture_old_color = texture_old.color();
			texture_new = std::move(texture_old);

			Assert::IsTrue(texture_new.color() == texture_old_color);
		}
	};
}