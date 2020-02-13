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

		glValidateProgram(old_program_id);

		glGetProgramiv(old_program_id, GL_VALIDATE_STATUS, &valid_status);
		BOOST_CHECK(valid_status == 1);

		glGetProgramiv(old_program_id, GL_ACTIVE_UNIFORMS, &num_attached_uniforms);
		BOOST_CHECK(num_attached_uniforms != 0);

		glGetProgramiv(old_program_id, GL_DELETE_STATUS, &delete_status);
		BOOST_CHECK(delete_status == 0);
	}

	glGetProgramiv(old_program_id, GL_DELETE_STATUS, &delete_status);
	BOOST_CHECK(delete_status == 1);

}

BOOST_AUTO_TEST_SUITE_END()