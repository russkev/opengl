#include "pch.h"

//#define BOOST_TEST_MODULE VectorUtils_test
#include <boost/test/unit_test.hpp>

//#include "Helper.h"

#include <glm/gtc/type_ptr.hpp>

#include "../gl_engine/utils/VectorUtils.h"

//namespace Helper
//{
//	bool matrix_is_similar(glm::mat4, glm::mat4, float);
//}

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;

void matricesAreSimilar(glm::mat4 result, glm::mat4 expected);

BOOST_AUTO_TEST_CASE(test_minValue)
{
	BOOST_TEST(gl_engine::VectorUtils::min_value(1, 2) == 1);
	BOOST_TEST(gl_engine::VectorUtils::min_value(1, 2, 3, 4, 5) == 1);
	BOOST_TEST(gl_engine::VectorUtils::min_value(0, 0) == 0);
	BOOST_TEST(gl_engine::VectorUtils::min_value(0, 0, 0, 0) == 0);
}

BOOST_AUTO_TEST_CASE(test_trs)
{
	{ // Test 1
		glm::vec3 t = { 0.0, 0.0, 0.0 };
		glm::vec3 r = { 0.0, 0.0, 0.0 };
		glm::vec3 s = { 1.0, 1.0, 1.0 };
		glm::mat4 result1 = gl_engine::VectorUtils::translate_rotate_scale(glm::mat3(t, r, s));
		glm::mat4 expected = glm::mat4(1.0f);
		BOOST_CHECK(result1 == expected);
	}

	{ // Test 2
		glm::vec3 t = { 1.0, 2.0, 3.0 };
		glm::vec3 r = { 0.0, 0.0, 0.0 };
		glm::vec3 s = { 1.0, 1.0, 1.0 };
		glm::mat4 result2 = gl_engine::VectorUtils::translate_rotate_scale(glm::mat3(t, r, s));
		glm::mat4 expected = glm::mat4(1.0f);
		expected[3][0] = 1.0;
		expected[3][1] = 2.0;
		expected[3][2] = 3.0;
		BOOST_CHECK(result2 == expected);
	}

	{ // Test3
		glm::vec3 t = { 1.0, 2.0, 3.0 };
		glm::vec3 r = { 0.0, 0.0, 0.0 };
		glm::vec3 s = { 1.0, 1.0, 1.0 };
		glm::mat4 result3 = gl_engine::VectorUtils::translate_rotate_scale(glm::mat3(t, r, s));
		glm::mat4 expected = glm::mat4(1.0f);
		expected[3][0] = 1.0;
		expected[3][1] = 2.0;
		expected[3][2] = 3.0;
		BOOST_CHECK(result3 == expected);
	}

	{ // Test4
		glm::vec3 t = { 0.0, 0.0, 0.0 };
		glm::vec3 r = { 30.0, 0.0, 0.0 };
		glm::vec3 s = { 1.0, 1.0, 1.0 };
		glm::mat4 result4 = gl_engine::VectorUtils::translate_rotate_scale(glm::mat3(t, r, s));


		float result_array[16] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.8660f, 0.5f, 0.0f,
			0.0f, -0.5f,0.8660f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		glm::mat4 expected = glm::make_mat4(result_array);

		//matricesAreSimilar(result4, expected);
		Helper::matrix_is_similar(result4, expected, 0.1f);
	}
}


void matricesAreSimilar(glm::mat4 result, glm::mat4 expected)
{
	for (auto i = 0; i < 4; ++i)
	{
		for (auto j = 0; j < 4; ++j)
		{
			BOOST_TEST(result[i][j] == expected[i][j], tt::tolerance(0.1f));
		}
	}

}