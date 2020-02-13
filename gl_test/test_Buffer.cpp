#include "pch.h"
#include <boost/test/unit_test.hpp>

#include "resources/Buffer.h"
#include "mesh/Mesh.h"
#include "mesh/Cylinder.h"

struct BufferTestContext
{
	BufferTestContext()
	{
		m_buffer.append(m_mesh.vertices());
	}

	GLint check_buffer_binding(GLuint id)
	{
		glBindBuffer(m_target, id);
		GLint which_id;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &which_id);
		glBindBuffer(m_target, 0);
		return which_id;
	}

	GLenum m_target = GL_ARRAY_BUFFER;
	glen::Window window{ "Test window", 800u, 600u };
	glen::Mesh m_mesh{ glen::Cylinder::create_cylinder(2.0f, 5.0f) };
	glen::Buffer m_buffer = { m_target, 0 };
};

BOOST_FIXTURE_TEST_SUITE(Buffer, BufferTestContext)

BOOST_AUTO_TEST_CASE(Destructor)
{
	GLuint old_id = 0;
	{
		glen::Buffer buffer{ m_target, 0 };
		buffer.append(m_mesh.vertices());

		old_id = buffer.buffer_id();

		BOOST_CHECK(old_id != 0);
		BOOST_CHECK(check_buffer_binding(old_id) == old_id);
	}

	BOOST_CHECK(check_buffer_binding(old_id) == 0);
}

BOOST_AUTO_TEST_CASE(Move_Constructor)
{
	GLuint old_id = m_buffer.buffer_id();
	size_t old_capacity = m_buffer.capacity();
	size_t old_size = m_buffer.size();

	BOOST_CHECK(old_id != 0);
	BOOST_CHECK(old_capacity != 0);
	BOOST_CHECK(old_size != 0);

	glen::Buffer new_buffer{ std::move(m_buffer) };

	BOOST_CHECK(check_buffer_binding(old_id) == new_buffer.buffer_id());
	BOOST_CHECK(new_buffer.capacity() == old_capacity);
	BOOST_CHECK(new_buffer.size() == old_size);

	BOOST_CHECK(m_buffer.binding_id() == 0u);
	BOOST_CHECK(m_buffer.capacity() == 0u);
	BOOST_CHECK(m_buffer.size() == 0u);
}

BOOST_AUTO_TEST_CASE(Move_Assign)
{
	GLuint old_id = m_buffer.buffer_id();
	size_t old_capacity = m_buffer.capacity();
	size_t old_size = m_buffer.size();

	BOOST_CHECK(old_id != 0);
	BOOST_CHECK(old_capacity != 0);
	BOOST_CHECK(old_size != 0);

	glen::Buffer new_buffer = std::move(m_buffer);

	BOOST_CHECK(check_buffer_binding(old_id) == new_buffer.buffer_id());
	BOOST_CHECK(new_buffer.capacity() == old_capacity);
	BOOST_CHECK(new_buffer.size() == old_size);

	BOOST_CHECK(m_buffer.binding_id() == 0u);
	BOOST_CHECK(m_buffer.capacity() == 0u);
	BOOST_CHECK(m_buffer.size() == 0u);
}

BOOST_AUTO_TEST_SUITE_END()