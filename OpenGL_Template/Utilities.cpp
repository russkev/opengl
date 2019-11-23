#pragma once
#include <GL/glew.h>

#include "Utilities.h"

//// // ----- DISTANCE SQUARED ----- // //
GLfloat distanceSquared(glm::vec2 s_point_1, glm::vec2 s_point_2)
{
	return
		(s_point_2.x - s_point_1.x) * (s_point_2.x - s_point_1.x) +
		(s_point_2.y - s_point_1.y) * (s_point_2.y - s_point_1.y);
}

GLfloat Utilities::distanceSquared(glm::vec3 s_point_1, glm::vec3 s_point_2)
{
	return
		(s_point_2.x - s_point_1.x) * (s_point_2.x - s_point_1.x) +
		(s_point_2.y - s_point_1.y) * (s_point_2.y - s_point_1.y) +
		(s_point_2.z - s_point_1.z) * (s_point_2.z - s_point_1.z);

}