#include "pch.h"
#include <boost/test/unit_test.hpp>

#include "resources/VAO.h"
#include "mesh/Mesh.h"
#include "mesh/Cube.h"
#include "resources/Buffer.h"
#include "resources/GL_Tuple_Introspect.h"
#include "node/Node.h"



struct VAOTestContext
{
	VAOTestContext()
	{
		m_buffer.append(*m_cube.vertices());
		const std::array<glen::member_info_type, 7> mesh_vao_info = glen::gl_introspect_tuple<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint, glm::vec3, glm::vec3>>::get();
		m_vao.generate_VAO(m_buffer, 0, mesh_vao_info.data(), mesh_vao_info.data() + mesh_vao_info.size(), 0);
	}

	glen::Window window{ "Test Window", 800u, 600u };
	glen::VAO m_vao;
	glen::Mesh m_cube{ glen::Cube::create_cube(2.0f) };
	glen::Buffer m_buffer{ GL_ARRAY_BUFFER, 0 };

	GLint check_vao_binding(GLuint id)
	{
		glBindVertexArray(id);
		GLint which_id;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &which_id);
		glBindVertexArray(0);
		return which_id;
	}
};

BOOST_FIXTURE_TEST_SUITE(VAO, VAOTestContext)

BOOST_AUTO_TEST_CASE(Destructor)
{
	GLuint id = 0;

	{
		glen::VAO vao;
		vao.generate_id();
		id = vao.id();
		BOOST_CHECK(id != 0);

		vao.bind();
		BOOST_CHECK(check_vao_binding(id) == id);
	}

	BOOST_CHECK(check_vao_binding(id) == 0);
}

BOOST_AUTO_TEST_CASE(Move_Constructor)
{
	GLuint old_vao_id = m_vao.id();

	glen::VAO new_vao{ std::move(m_vao) };

	BOOST_CHECK(new_vao.id() == old_vao_id);
	BOOST_CHECK(check_vao_binding(old_vao_id) == new_vao.id());
}


BOOST_AUTO_TEST_CASE(Move_Assign)
{
	GLuint old_vao_id = m_vao.id();

	glen::VAO new_vao = std::move(m_vao);

	BOOST_CHECK(new_vao.id() == old_vao_id);
	BOOST_CHECK(check_vao_binding(old_vao_id) == new_vao.id());
}

BOOST_AUTO_TEST_SUITE_END()