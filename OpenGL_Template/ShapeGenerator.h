#pragma once

#include <glm/matrix.hpp>
#include <math.h>
#include <tuple>
#include <cassert>

#include "ShapeData.h"
#include "Vertex.h"

#define PI 3.1415927

typedef std::tuple<glm::vec3, glm::vec3, glm::vec3>		vertexType;
typedef std::vector<vertexType>							shapeType;
typedef std::vector<GLushort>							indexType;

struct ShapeGenerator
{
	// // CONSTRUCTOR
	//ShapeGenerator() : m_shapes(ShapeData()) {};

	// // GETTERS
	shapeType vertices();// { return m_shapes.vertices(); }
	//auto vertices()
	//{
	//	return m_shapes2.at(0).vertices();
	//}
	auto indices() { return m_shapes.indices(); }


	// // APPEND THE SHAPES
	void appendTriangle()						{ m_shapes += makeTriangle();		 m_shapes2.push_back(makeTriangle());			}
	void appendPlane(GLuint dimensions = 10)	{ m_shapes += makePlane(dimensions); m_shapes2.push_back(makePlane(dimensions));	}
	void appendCube()							{ m_shapes += makeCube();			 m_shapes2.push_back(makeCube());				}
	void appendArrow()							{ m_shapes += makeArrow();			 m_shapes2.push_back(makeArrow());				}
	void appendTube(GLuint resolution = 10, GLfloat radius = 2, GLfloat height = 2)
	{
		m_shapes += makeTube(resolution, radius, height);
		m_shapes2.push_back(makeTube(resolution, radius, height));
	}
	void appendNormals(ShapeData& inShape)		{ m_shapes += makeNormals(inShape);	 m_shapes2.push_back(makeNormals(inShape));		}
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

	std::vector<ShapeData> m_shapes2;

	// // CREATE THE SHAPES
	ShapeData makeTriangle();
	ShapeData makePlane(GLuint dimensions);
	ShapeData makeCube();
	ShapeData makeArrow();
	ShapeData makeTube(GLuint resolution, GLfloat radius, GLfloat height);
	ShapeData makeNormals(ShapeData& inShape);
};