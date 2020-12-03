#include "pch.h"
#include "CppUnitTest.h"

#include "resources/VAO.h"
#include "mesh/Mesh.h"
#include "mesh/Cube.h"
#include "resources/Buffer.h"
#include "resources/GL_Tuple_Introspect.h"
#include "node/Node.h"
#include "render/Window.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace gl_test
{
	TEST_CLASS(VAO)
	{
		glen::Window window{ "Test Window", 1u, 1u };
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

	public:
		TEST_METHOD_INITIALIZE(VAO_Init)
		{
			m_buffer.append(*m_cube.vertices());
			const std::array<glen::member_info_type, 7> mesh_vao_info = 
				glen::gl_introspect_tuple<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint, glm::vec3, glm::vec3>>::get();
			m_vao.generate_VAO(m_buffer, 0, mesh_vao_info.data(), mesh_vao_info.data() + mesh_vao_info.size(), 0);
		}

		TEST_METHOD(Destructor)
		{
			GLuint id = 0;

			{
				glen::VAO vao;
				vao.generate_id();
				id = vao.id();
				Assert::IsTrue(id != 0);

				vao.bind();
				Assert::IsTrue(check_vao_binding(id) == id);
			}

			Assert::IsTrue(check_vao_binding(id) == 0);
		}

		TEST_METHOD(Move_Constructor)
		{
			GLuint old_vao_id = m_vao.id();

			glen::VAO new_vao{ std::move(m_vao) };

			Assert::IsTrue(new_vao.id() == old_vao_id);
			Assert::IsTrue(check_vao_binding(old_vao_id) == new_vao.id());
		}


		TEST_METHOD(Move_Assign)
		{
			GLuint old_vao_id = m_vao.id();

			glen::VAO new_vao = std::move(m_vao);

			Assert::IsTrue(new_vao.id() == old_vao_id);
			Assert::IsTrue(check_vao_binding(old_vao_id) == new_vao.id());
		}
	};
}