#pragma once

#include <glm/matrix.hpp>
#include <math.h>
#include <tuple>
#include <cassert>

#include "ShapeData.h"
#include "Vertex.h"

#define PI 3.1415927

struct ShapeGenerator
{
	// // CONSTRUCTOR
	ShapeGenerator() : m_shapes(ShapeData()) {};

	// // GETTERS
	auto vertices() { return m_shapes.vertices(); }
	auto indices() { return m_shapes.indices(); }


	// // APPEND THE SHAPES
	void appendTriangle() { m_shapes += makeTriangle(); }
	void appendPlane(GLuint dimensions = 10) { m_shapes += makePlane(dimensions); }
	void appendCube() { m_shapes += makeCube(); }
	void appendArrow() { m_shapes += makeArrow(); }
	void appendTube(GLuint resolution = 10, GLfloat radius = 2, GLfloat height = 2)
	{
		m_shapes += makeTube(resolution, radius, height);
	}
	void appendNormals(ShapeData& inShape) { m_shapes += makeNormals(inShape); }
	void appendNormals()
	{
		assert(sizeof(m_shapes) > 0);
		m_shapes += makeNormals(m_shapes);
	}

	// // TRANSFORM
	void transform(glm::mat4 transformMatrix) { m_shapes.transform(transformMatrix); }

	// // RANDOM COLOR
	glm::vec3 randomColor();

private:
	// // MEMBER VARIABLES
	ShapeData m_shapes;

	// // CREATE THE SHAPES
	ShapeData makeTriangle();
	ShapeData makePlane(GLuint dimensions);
	ShapeData makeCube();
	ShapeData makeArrow();
	ShapeData makeTube(GLuint resolution, GLfloat radius, GLfloat height);
	ShapeData makeNormals(ShapeData& inShape);
};