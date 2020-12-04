#include "pch.h"
#include "CppUnitTest.h"

#include "render/Window.h"
#include "material/Material.h"
#include "material/MaterialLibrary.h"
#include "Shading/Texture.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace gl_test
{
	TEST_CLASS(Material)
	{
		glen::Window window{ "Window", 1u, 1u };
		glen::BlinnMaterial m_material{};

		glen::Texture grid_tex{ (glen::Material::shaders_dir() + "..\\textures\\utility\\grey_grid.tga").c_str() };
		glen::Texture brick_normals{ (glen::Material::shaders_dir() + "..\\textures\\bricks\\bricks_normal.tga").c_str() };

	public:
		TEST_METHOD_INITIALIZE(Material_Init)
		{
			m_material.set_texture(m_material.k_material_diffuse, &grid_tex);
			m_material.set_texture(m_material.k_material_specular, &brick_normals);
			m_material.set_uniform(m_material.k_spot_light + "[0]." + m_material.k_brightness, 800.0f);
			m_material.set_uniform(m_material.k_material_normal, glm::vec3{ 0.5f, 0.5f, 1.0f });
		}

		TEST_METHOD(Destructor)
		{
			GLuint old_program_id = 0;
			GLint valid_status = 0, num_attached_uniforms = 0, delete_status = 0;

			{
				glen::BlinnMaterial material{ std::move(m_material) };
				old_program_id = material.program_id();
				Assert::IsTrue(old_program_id != 0);

				glGetProgramiv(old_program_id, GL_ACTIVE_UNIFORMS, &num_attached_uniforms);
				Assert::IsTrue(num_attached_uniforms != 0);

				glGetProgramiv(old_program_id, GL_DELETE_STATUS, &delete_status);
				Assert::IsTrue(delete_status == 0);
			}

			glGetProgramiv(old_program_id, GL_DELETE_STATUS, &delete_status);
			Assert::IsTrue(delete_status == 1);
		}

		TEST_METHOD(Move_Constructor)
		{
			glen::BlinnMaterial old_material{ std::move(m_material) };
			GLuint old_id = old_material.program_id();
			GLuint old_num_uniforms = old_material.num_uniforms();
			GLuint old_num_textures = old_material.num_textures();
			GLint delete_status = 0;

			Assert::IsTrue(old_num_uniforms != 0);
			Assert::IsTrue(old_num_textures != 0);

			glen::BlinnMaterial new_material{ std::move(old_material) };

			Assert::IsTrue(new_material.program_id() == old_id);
			Assert::IsTrue(new_material.num_textures() == old_num_textures);
			Assert::IsTrue(new_material.num_uniforms() == old_num_uniforms);

			glValidateProgram(new_material.program_id());

			glGetProgramiv(new_material.program_id(), GL_DELETE_STATUS, &delete_status);
			Assert::IsTrue(delete_status == 0);
		}

		TEST_METHOD(Move_Assign)
		{
			glen::BlinnMaterial old_material{ std::move(m_material) };
			GLuint old_id = old_material.program_id();
			GLuint old_num_uniforms = old_material.num_uniforms();
			GLuint old_num_textures = old_material.num_textures();
			GLint delete_status = 0;

			Assert::IsTrue(old_num_uniforms != 0);
			Assert::IsTrue(old_num_textures != 0);

			glen::BlinnMaterial new_material = std::move(old_material);

			Assert::IsTrue(new_material.program_id() == old_id);
			Assert::IsTrue(new_material.num_textures() == old_num_textures);
			Assert::IsTrue(new_material.num_uniforms() == old_num_uniforms);

			glValidateProgram(new_material.program_id());

			glGetProgramiv(new_material.program_id(), GL_DELETE_STATUS, &delete_status);
			Assert::IsTrue(delete_status == 0);
		}
	};

}