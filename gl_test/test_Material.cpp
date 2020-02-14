#include "pch.h"
#include <boost/test/unit_test.hpp>

#include "material/Material.h"
#include "material/MaterialLibrary.h"
#include "Shading/Texture.h"

struct MaterialTestContext
{
	glen::BlinnMaterial create_test_material()
	{

		glen::BlinnMaterial material{};
		glen::Texture grid_tex{ "greyGrid_01.tga" };
		glen::Texture brick_normals{ "bricks2_normal.tga" };

		material.set_texture(material.k_material_diffuse, &grid_tex);
		material.set_texture(material.k_material_specular, &brick_normals);
		material.set_uniform(material.k_spot_light + "[0]." + material.k_brightness, 800.0f);
		material.set_uniform(material.k_material_normal, glm::vec3{ 0.5f, 0.5f, 1.0f });
		return material;
	}

	glen::Window window{ "Test Window", 800u, 600u };
};

BOOST_FIXTURE_TEST_SUITE(Material, MaterialTestContext)

BOOST_AUTO_TEST_CASE(Destructor)
{
	GLuint old_program_id = 0;
	GLint valid_status = 0, num_attached_uniforms = 0, delete_status = 0;

	{
		glen::BlinnMaterial material{ std::move(create_test_material()) };
		old_program_id = material.program_id();
		BOOST_CHECK(old_program_id != 0);

		glGetProgramiv(old_program_id, GL_ACTIVE_UNIFORMS, &num_attached_uniforms);
		BOOST_CHECK(num_attached_uniforms != 0);

		glGetProgramiv(old_program_id, GL_DELETE_STATUS, &delete_status);
		BOOST_CHECK(delete_status == 0);
	}

	glGetProgramiv(old_program_id, GL_DELETE_STATUS, &delete_status);
	BOOST_CHECK(delete_status == 1);

}

BOOST_AUTO_TEST_CASE(Move_Constructor)
{
	glen::BlinnMaterial old_material{ std::move(create_test_material()) };
	GLuint old_id = old_material.program_id();
	GLuint old_num_uniforms = old_material.num_uniforms();
	GLuint old_num_textures = old_material.num_textures();
	GLint delete_status = 0;

	BOOST_CHECK(old_num_uniforms != 0);
	BOOST_CHECK(old_num_textures != 0);

	glen::BlinnMaterial new_material{ std::move(old_material) };

	BOOST_CHECK(new_material.program_id() == old_id);
	BOOST_CHECK(new_material.num_textures() == old_num_textures);
	BOOST_CHECK(new_material.num_uniforms() == old_num_uniforms);

	glValidateProgram(new_material.program_id());

	glGetProgramiv(new_material.program_id(), GL_DELETE_STATUS, &delete_status);
	BOOST_CHECK(delete_status == 0);
}

BOOST_AUTO_TEST_CASE(Move_Assign)
{
	glen::BlinnMaterial old_material{ std::move(create_test_material()) };
	GLuint old_id = old_material.program_id();
	GLuint old_num_uniforms = old_material.num_uniforms();
	GLuint old_num_textures = old_material.num_textures();
	GLint delete_status = 0;

	BOOST_CHECK(old_num_uniforms != 0);
	BOOST_CHECK(old_num_textures != 0);

	glen::BlinnMaterial new_material = std::move(old_material);

	BOOST_CHECK(new_material.program_id() == old_id);
	BOOST_CHECK(new_material.num_textures() == old_num_textures);
	BOOST_CHECK(new_material.num_uniforms() == old_num_uniforms);

	glValidateProgram(new_material.program_id());

	glGetProgramiv(new_material.program_id(), GL_DELETE_STATUS, &delete_status);
	BOOST_CHECK(delete_status == 0);
}

BOOST_AUTO_TEST_SUITE_END()