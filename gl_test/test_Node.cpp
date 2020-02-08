#pragma once
#include "pch.h"
#include <boost/test/unit_test.hpp>

#include "../gl_engine/node/Node.h"
//#include "../gl_engine/node/Node.cpp"

#include <glm/glm.hpp>


//namespace Helper
//{
//	bool matrix_is_similar(glm::mat4, glm::mat4, float);
//}

namespace tt = boost::test_tools;

struct NodeTestContext
{
	NodeTestContext() :
		N1(glen::Node("N1")),
		N2(glen::Node("N2")),
		N3(glen::Node("N3")),
		N4(glen::Node("N4"))
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

	glen::Node N1;
	glen::Node N2;
	glen::Node N3;
	glen::Node N4;
	
	glm::mat4 expectedMatrix1;
	glm::mat4 expectedMatrix2;

};

//----------------------------------------------------------------------------------------------------//


BOOST_FIXTURE_TEST_SUITE(Node_tests, NodeTestContext)

BOOST_AUTO_TEST_SUITE(transform_tests)

BOOST_AUTO_TEST_CASE(N1_local)
{
	BOOST_TEST(Helper::matrix_is_similar(*N1.local_to_node(), expectedMatrix1, 0.1f));
}
BOOST_AUTO_TEST_CASE(N1_world)
{
	BOOST_TEST(Helper::matrix_is_similar(N1.world_to_node(), expectedMatrix1, 0.1f));
}
BOOST_AUTO_TEST_CASE(N2_world)
{
	BOOST_TEST(Helper::matrix_is_similar(N2.world_to_node(), expectedMatrix1, 0.1f));
}
BOOST_AUTO_TEST_CASE(N3_world)
{
	BOOST_TEST(Helper::matrix_is_similar(N3.world_to_node(), expectedMatrix1, 0.1f));
}
BOOST_AUTO_TEST_CASE(N4_local)
{
	BOOST_TEST(Helper::matrix_is_similar(*N4.local_to_node(), expectedMatrix2, 0.1f));
}
BOOST_AUTO_TEST_CASE(N4_world)
{
	auto finalExpectedMatrix = expectedMatrix1 * expectedMatrix2;
	BOOST_TEST(Helper::matrix_is_similar(N4.world_to_node(), finalExpectedMatrix, 0.1f));
}

BOOST_AUTO_TEST_SUITE_END()

//----------------------------------------------------------------------------------------------------//

BOOST_AUTO_TEST_SUITE(relationship_tests)

BOOST_AUTO_TEST_CASE(N1_relationships)
{
	BOOST_TEST(N1.children().size() == 1);
	auto children = N1.children();
	BOOST_CHECK(children["N2"] == &N2);
	BOOST_CHECK(N1.parent() == false);
	glen::Node newNode = glen::Node("N2");
	BOOST_CHECK_THROW(N1.add_child(&newNode), std::runtime_error);
}
BOOST_AUTO_TEST_CASE(N2_relationships)
{
	BOOST_TEST(N2.children().size() == 2);
	auto children = N2.children();
	BOOST_TEST(children["N3"] == &N3);
	BOOST_TEST(children["N4"] == &N4);
	BOOST_TEST(N2.parent() == &N1);
}
BOOST_AUTO_TEST_CASE(N2_disconnect)
{
	glen::Node* N2_b = N1.disconnect_child("N2");
	BOOST_TEST(N2_b == &N2);
	BOOST_CHECK(N2.parent() == false);
	BOOST_TEST(N2.children().size() == 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()