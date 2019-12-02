#pragma once
#include "pch.h"
//#define BOOST_TEST_MODULE mytests
#include <boost/test/unit_test.hpp>
#include "../gl_engine/node/Node.h"
#include "../gl_engine/node/Node.cpp"

#include <glm/glm.hpp>
#include "../gl_engine/utils/VectorUtils.h"

namespace tt = boost::test_tools;

void matrixIsSimilar(glm::mat4 result, glm::mat4 expected)
{
	for (auto i = 0; i < 4; ++i)
	{
		for (auto j = 0; j < 4; ++j)
		{
			BOOST_TEST(result[i][j] == expected[i][j], tt::tolerance(0.1f));
		}
	}

}

BOOST_AUTO_TEST_CASE(test_node)
{
	Node N1("N1");
	Node N2("N2");
	Node N3("N3");
	Node N4("N4");

	N1.addChild(&N2);
	N2.addChild(&N3);
	N2.addChild(&N4);

	N1.position() = { 3.0f, 2.0f, 1.0f };

	glm::mat4 expectedMatrix = glm::mat4(1.0f);
	expectedMatrix[3][0] = 3.0f;
	expectedMatrix[3][1] = 2.0f;
	expectedMatrix[3][2] = 1.0f;

	matrixIsSimilar(N1.localTransform(), expectedMatrix);
	matrixIsSimilar(N1.worldTransform(), expectedMatrix);
	matrixIsSimilar(N2.worldTransform(), expectedMatrix);
	matrixIsSimilar(N3.worldTransform(), expectedMatrix);
	matrixIsSimilar(N4.worldTransform(), expectedMatrix);



}