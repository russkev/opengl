#pragma once

#include <glm/matrix.hpp>
#include <math.h>
#include <tuple>
#include <cassert>
#include <algorithm>

#include "ShapeData.h"
#include "Vertex.h"

#define PI 3.1415927

struct ShapeGenerator
{
	// // GETTERS
	ShapeData::verticesType vertices();// { return m_shapes.vertices(); }
	ShapeData::indicesType indices();// { return m_shapes.indices(); }


	// // APPEND THE SHAPES
	void appendShape(ShapeData& inShape);
	void appendTriangle();
	void appendPlane(GLuint dimensions);
	void appendCube();
	void appendArrow();
	void appendTube(GLuint resolution, GLfloat radius, GLfloat height);
	void appendNormals(ShapeData& inShape);
	void appendNormals();

	// // TRANSFORM
	void transform(glm::mat4 transformMatrix);

	// // RANDOM COLOR
	glm::vec3 randomColor();

private:
	// // MEMBER VARIABLES
	//ShapeData m_shapes;
	std::vector<ShapeData> m_shapes2;

	// // CREATE THE SHAPES
	ShapeData makeTriangle();
	ShapeData makePlane(GLuint dimensions);
	ShapeData makeCube();
	ShapeData makeArrow();
	ShapeData makeTube(GLuint resolution, GLfloat radius, GLfloat height);
	ShapeData makeNormals(ShapeData& inShape);
};