#include "pch.h"
#include "CppUnitTest.h"

#include "resources/Buffer.h"
#include "mesh/Mesh.h"
#include "mesh/Cylinder.h"
#include "render/Window.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace gl_test
{
	TEST_CLASS(Buffer)
	{
		GLint check_buffer_binding(GLuint id)
		{
			glBindBuffer(m_target, id);
			GLint which_id;
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &which_id);
			glBindBuffer(m_target, 0);
			return which_id;
		}

		GLenum m_target = GL_ARRAY_BUFFER;
		glen::Window m_window{ "Test window", 1u, 1u };
		glen::Mesh m_mesh{ glen::Cylinder::create_cylinder(2.0f, 5.0f) };
		glen::Buffer m_buffer = { m_target, 0 };

	public:
		TEST_METHOD_INITIALIZE(init)
		{
			m_buffer.append(*m_mesh.vertices());
		}
		TEST_METHOD(Destructor)
		{
			GLuint old_id = 0;
			{
				glen::Buffer buffer{ m_target, 0 };
				buffer.append(*m_mesh.vertices());

				old_id = buffer.buffer_id();

				Assert::IsTrue(old_id != 0);
				Assert::IsTrue(check_buffer_binding(old_id) == old_id);
			}
			Assert::IsTrue(check_buffer_binding(old_id) == 0);
		}
		TEST_METHOD(Move_Constructor)
		{
			GLuint old_id = m_buffer.buffer_id();
			size_t old_capacity = m_buffer.capacity();
			size_t old_size = m_buffer.size();

			Assert::IsTrue(old_id != 0);
			Assert::IsTrue(old_capacity != 0);
			Assert::IsTrue(old_size != 0);

			glen::Buffer new_buffer{ std::move(m_buffer) };

			Assert::IsTrue(check_buffer_binding(old_id) == new_buffer.buffer_id());
			Assert::IsTrue(new_buffer.capacity() == old_capacity);
			Assert::IsTrue(new_buffer.size() == old_size);

			Assert::IsTrue(m_buffer.binding_id() == 0u);
			Assert::IsTrue(m_buffer.capacity() == 0u);
			Assert::IsTrue(m_buffer.size() == 0u);
		}
		TEST_METHOD(Move_Assign)
		{
			GLuint old_id = m_buffer.buffer_id();
			size_t old_capacity = m_buffer.capacity();
			size_t old_size = m_buffer.size();

			(old_id != 0);
			(old_capacity != 0);
			(old_size != 0);

			glen::Buffer new_buffer = std::move(m_buffer);

			Assert::IsTrue(check_buffer_binding(old_id) == new_buffer.buffer_id());
			Assert::IsTrue(new_buffer.capacity() == old_capacity);
			Assert::IsTrue(new_buffer.size() == old_size);

			Assert::IsTrue(m_buffer.binding_id() == 0u);
			Assert::IsTrue(m_buffer.capacity() == 0u);
			Assert::IsTrue(m_buffer.size() == 0u);
		}
	};
}
