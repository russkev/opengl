#include "pch.h"
#include "CppUnitTest.h"

#include "utils/VectorUtils.h"
#include "utils/ScalarUtils.h"
#include "helper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace gl_test
{
	TEST_CLASS(VectorUtils)
	{

	public:
		TEST_METHOD(Min_Value)
		{
			Assert::IsTrue(glen::ScalarUtils::min_value(1, 2) == 1);
			Assert::IsTrue(glen::ScalarUtils::min_value(1, 2, 3, 4, 5) == 1);
			Assert::IsTrue(glen::ScalarUtils::min_value(0, 0) == 0);
			Assert::IsTrue(glen::ScalarUtils::min_value(0, 0, 0, 0) == 0);
		}

		TEST_METHOD(Lerp)
		{
			Assert::IsTrue(glen::ScalarUtils::lerp(0.0f, 4.0f, 0.25f) == 1.0f);
		}

		TEST_METHOD(TRS)
		{
			{ // Test 1
				glm::vec3 t = { 0.0, 0.0, 0.0 };
				glm::vec3 r = { 0.0, 0.0, 0.0 };
				glm::vec3 s = { 1.0, 1.0, 1.0 };
				glm::mat4 result1 = glen::VectorUtils::translate_rotate_scale(glm::mat3(t, r, s));
				glm::mat4 expected = glm::mat4(1.0f);
				Assert::IsTrue(result1 == expected);
			}

			{ // Test 2
				glm::vec3 t = { 1.0, 2.0, 3.0 };
				glm::vec3 r = { 0.0, 0.0, 0.0 };
				glm::vec3 s = { 1.0, 1.0, 1.0 };
				glm::mat4 result2 = glen::VectorUtils::translate_rotate_scale(glm::mat3(t, r, s));
				glm::mat4 expected = glm::mat4(1.0f);
				expected[3][0] = 1.0;
				expected[3][1] = 2.0;
				expected[3][2] = 3.0;
				Assert::IsTrue(result2 == expected);
			}

			{ // Test3
				glm::vec3 t = { 1.0, 2.0, 3.0 };
				glm::vec3 r = { 0.0, 0.0, 0.0 };
				glm::vec3 s = { 1.0, 1.0, 1.0 };
				glm::mat4 result3 = glen::VectorUtils::translate_rotate_scale(glm::mat3(t, r, s));
				glm::mat4 expected = glm::mat4(1.0f);
				expected[3][0] = 1.0;
				expected[3][1] = 2.0;
				expected[3][2] = 3.0;
				Assert::IsTrue(result3 == expected);
			}

			{ // Test4
				glm::vec3 t = { 0.0, 0.0, 0.0 };
				glm::vec3 r = { 30.0, 0.0, 0.0 };
				glm::vec3 s = { 1.0, 1.0, 1.0 };
				glm::mat4 result4 = glen::VectorUtils::translate_rotate_scale(glm::mat3(t, r, s));


				float result_array[16] = {
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.8660f, 0.5f, 0.0f,
					0.0f, -0.5f,0.8660f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				};

				glm::mat4 expected = glm::make_mat4(result_array);

				Assert::IsTrue(Helper::matrix_is_similar(result4, expected, 0.1f));
			}
		}
	};
}