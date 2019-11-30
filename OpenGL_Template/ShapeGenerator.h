#pragma once

#include <glm/matrix.hpp>
#include <math.h>
#include <tuple>
#include <cassert>
#include <algorithm>

#include "ShapeData.h"

#define PI 3.1415927

struct ShapeGenerator
{
	// // GETTERS
	ShapeData::verticesType vertices();
	ShapeData::indicesType indices();

	// // TRANSFORM
	void transform(glm::mat4 transformMatrix);

	// // RANDOM COLOR
	glm::vec3 randomColor();

public:
	// // MEMBER VARIABLES
	std::vector<ShapeData> m_shapes;

	// // CREATE THE SHAPES
	ShapeData makeTriangle();
	ShapeData makePlane(GLuint dimensions);
	ShapeData makeCube();
	ShapeData makeArrow();
	ShapeData makeTube(GLuint resolution, GLfloat radius, GLfloat height);
	//ShapeData makeNormals(ShapeData& inShape);
};