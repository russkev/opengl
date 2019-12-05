#pragma once
#include "pch.h"
#include "helper.h"
#include <boost/test/unit_test.hpp>
#include "../gl_engine/node/Node.h"
#include "../gl_engine/node/Node.cpp"

#include <glm/glm.hpp>
#include "../gl_engine/utils/VectorUtils.h"

namespace tt = boost::test_tools;

struct nodeTestContext
{
	nodeTestContext() :
		N1(Node("N1")),
		N2(Node("N2")),
		N3(Node("N3")),
		N4(Node("N4"))
	{
		N1.addChild(&N2);
		N2.addChild(&N3);
		N2.addChild(&N4);

		N1.setPosition({ 3.0f, 2.0f, 1.0f });
		N4.setPosition({ 2.0f, 1.0f, 3.0f });

		expectedMatrix1 = glm::mat4(1.0f);
		expectedMatrix1[3][0] = 3.0f;
		expectedMatrix1[3][1] = 2.0f;
		expectedMatrix1[3][2] = 1.0f;

		expectedMatrix2 = glm::mat4(1.0f);
		expectedMatrix2[3][0] = 2.0f;
		expectedMatrix2[3][1] = 1.0f;
		expectedMatrix2[3][2] = 3.0f;
	}

	Node N1;
	Node N2;
	Node N3;
	Node N4;

	glm::mat4 expectedMatrix1;
	glm::mat4 expectedMatrix2;

};

//----------------------------------------------------------------------------------------------------//


BOOST_FIXTURE_TEST_SUITE(nodeTests, nodeTestContext)

BOOST_AUTO_TEST_SUITE(transfromTests)

BOOST_AUTO_TEST_CASE(N1_local)
{
	BOOST_TEST(Helper::matrixIsSimilar(N1.localTransform(), expectedMatrix1, 0.1f));
}
BOOST_AUTO_TEST_CASE(N1_world)
{
	BOOST_TEST(Helper::matrixIsSimilar(N1.worldTransform(), expectedMatrix1, 0.1f));
}
BOOST_AUTO_TEST_CASE(N2_world)
{
	BOOST_TEST(Helper::matrixIsSimilar(N2.worldTransform(), expectedMatrix1, 0.1f));
}
BOOST_AUTO_TEST_CASE(N3_world)
{
	BOOST_TEST(Helper::matrixIsSimilar(N3.worldTransform(), expectedMatrix1, 0.1f));
}
BOOST_AUTO_TEST_CASE(N4_local)
{
	BOOST_TEST(Helper::matrixIsSimilar(N4.localTransform(), expectedMatrix2, 0.1f));
}
BOOST_AUTO_TEST_CASE(N4_world)
{
	auto finalExpectedMatrix = expectedMatrix1 * expectedMatrix2;
	BOOST_TEST(Helper::matrixIsSimilar(N4.worldTransform(), finalExpectedMatrix, 0.1f));
}

BOOST_AUTO_TEST_SUITE_END()

//----------------------------------------------------------------------------------------------------//

BOOST_AUTO_TEST_SUITE(relationshipTests)

BOOST_AUTO_TEST_CASE(N1_relationships)
{
	BOOST_TEST(N1.children().size() == 1);
	auto children = N1.children();
	BOOST_CHECK(children["N2"] == &N2);
	BOOST_CHECK(N1.parent() == false);
	Node newNode = Node("N2");
	BOOST_CHECK_THROW(N1.addChild(&newNode), std::runtime_error);
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
	Node* N2_b = N1.disconnectChild("N2");
	BOOST_TEST(N2_b == &N2);
	BOOST_CHECK(N2.parent() == false);
	BOOST_TEST(N2.children().size() == 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()