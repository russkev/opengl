#pragma once
#include <cmath>
#include <GL/glew.h>

#include "VectorUtils.h"

// // ----- DISTANCE SQUARED ----- // //
GLfloat VectorUtils::distanceSquared(glm::vec2 s_point_1, glm::vec2 s_point_2)
{
	return
		(s_point_2.x - s_point_1.x) * (s_point_2.x - s_point_1.x) +
		(s_point_2.y - s_point_1.y) * (s_point_2.y - s_point_1.y);
}

GLfloat VectorUtils::distanceSquared(glm::vec3 s_point_1, glm::vec3 s_point_2)
{
	return
		(s_point_2.x - s_point_1.x) * (s_point_2.x - s_point_1.x) +
		(s_point_2.y - s_point_1.y) * (s_point_2.y - s_point_1.y) +
		(s_point_2.z - s_point_1.z) * (s_point_2.z - s_point_1.z);

}

// // ----- IS LESS THAN ----- // //
bool VectorUtils::isLessThan(const glm::vec2& v1, const glm::vec2& v2, const float threshold)
{
	if (
		std::abs(v1.x - v2.x) < threshold &&
		std::abs(v1.y - v2.y) < threshold
		)
	{
		return false;
	}
	else
	{
		return v1.x < v2.x && v1.y < v2.y;
	}
}

bool VectorUtils::isLessThan(const glm::vec3& v1, const glm::vec3& v2, const float threshold)
{
	if (
		std::abs(v1.x - v2.x) < threshold &&
		std::abs(v1.y - v2.y) < threshold &&
		std::abs(v1.z - v2.z) < threshold
		)
	{
		return false;
	}
	else
	{
		return v1.x < v2.x && v1.y < v2.y && v1.z < v2.z;
	}
}