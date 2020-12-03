#include "pch.h"
#include "CppUnitTest.h"

#include "node/Node.h"
#include "helper.h"

#include <glm/glm.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace gl_test
{
	TEST_CLASS(Node)
	{
		glen::Node N1{ "N1" };
		glen::Node N2{ "N2" };
		glen::Node N3{ "N3" };
		glen::Node N4{ "N4" };

		glm::mat4 expectedMatrix1;
		glm::mat4 expectedMatrix2;

	public:
		TEST_METHOD_INITIALIZE(Node_Init)
		{
			N1.add_child(&N2);
			N2.add_child(&N3);
			N2.add_child(&N4);

			N1.set_position({ 3.0f, 2.0f, 1.0f });
			N4.set_position({ 2.0f, 1.0f, 3.0f });

			expectedMatrix1 = glm::mat4(1.0f);
			expectedMatrix1[3][0] = 3.0f;
			expectedMatrix1[3][1] = 2.0f;
			expectedMatrix1[3][2] = 1.0f;

			expectedMatrix2 = glm::mat4(1.0f);
			expectedMatrix2[3][0] = 2.0f;
			expectedMatrix2[3][1] = 1.0f;
			expectedMatrix2[3][2] = 3.0f;
		}
		TEST_METHOD(N1_local)
		{
			Assert::IsTrue(Helper::matrix_is_similar(*N1.local_to_node(), expectedMatrix1, 0.1f));
		}
		TEST_METHOD(N1_world)
		{
			Assert::IsTrue(Helper::matrix_is_similar(N1.world_to_node(), expectedMatrix1, 0.1f));
		}
		TEST_METHOD(N2_world)
		{
			Assert::IsTrue(Helper::matrix_is_similar(N2.world_to_node(), expectedMatrix1, 0.1f));
		}
		TEST_METHOD(N3_world)
		{
			Assert::IsTrue(Helper::matrix_is_similar(N3.world_to_node(), expectedMatrix1, 0.1f));
		}
		TEST_METHOD(N4_local)
		{
			Assert::IsTrue(Helper::matrix_is_similar(*N4.local_to_node(), expectedMatrix2, 0.1f));
		}
		TEST_METHOD(N4_world)
		{
			auto finalExpectedMatrix = expectedMatrix1 * expectedMatrix2;
			Assert::IsTrue(Helper::matrix_is_similar(N4.world_to_node(), finalExpectedMatrix, 0.1f));
		}

		TEST_METHOD(N1_relationships)
		{
			Assert::IsTrue(N1.children().size() == 1);
			auto children = N1.children();
			Assert::IsTrue(children["N2"] == &N2);
			Assert::IsTrue(N1.parent() == NULL);
			glen::Node newNode = glen::Node("N2");

			auto add_child_func = [&newNode, this] { N1.add_child(&newNode); };
			Assert::ExpectException<std::runtime_error>(add_child_func);
		}
		TEST_METHOD(N2_relationships)
		{
			Assert::IsTrue(N2.children().size() == 2);
			auto children = N2.children();
			Assert::IsTrue(children["N3"] == &N3);
			Assert::IsTrue(children["N4"] == &N4);
			Assert::IsTrue(N2.parent() == &N1);
		}
		TEST_METHOD(N2_disconnect)
		{
			glen::Node* N2_b = N1.disconnect_child("N2");
			Assert::IsTrue(N2_b == &N2);
			Assert::IsTrue(N2.parent() == NULL);
			Assert::IsTrue(N2.children().size() == 2);
		}
	};
}