#pragma once

#include "ShapeGenerator.h"
#include "ShapeData.h"
#include "Vertex.h"

#include <glm/matrix.hpp>
#include <math.h>
#include <tuple>
#include <cassert>

// // ----- Getters ----- // //
ShapeData::verticesType ShapeGenerator::vertices() 
{
	
	assert(m_shapes.size() > 0);
	ShapeData::verticesType t_vertices = m_shapes.at(0).vertices();
	if (m_shapes.size() > 1)
	{
		for (auto i = 1; i < m_shapes.size(); ++i)
		{
			t_vertices.insert(t_vertices.end(), m_shapes.at(i).vert_begin(), m_shapes.at(i).vert_end());
		}
	}
	return t_vertices;
}

ShapeData::indicesType ShapeGenerator::indices()
{
	assert(m_shapes.size() > 0);
	ShapeData::indicesType t_indices = m_shapes.at(0).indices();
	if (m_shapes.size() > 1)
	{
		for (auto i = 1; i < m_shapes.size(); ++i) 
		{
			t_indices.insert(t_indices.end(), m_shapes.at(i).indx_begin(), m_shapes.at(i).indx_end());
		}
	}
	return t_indices;
}

// // TRANSFORM
void ShapeGenerator::transform(glm::mat4 transformMatrix) 
{
	for (auto & i : m_shapes)
	{
		i.transform(transformMatrix);
	}
}

// // ----- Append Shapes ----- // //
//void ShapeGenerator::appendShape(ShapeData& inShape)
//{
//	if (m_shapes.size() > 0)
//	{
//		std::size_t vertexCount = 0;
//		for (auto & i : m_shapes)
//		{ 
//			vertexCount += i.numVertices(); 
//		}
//
//		for (auto i = 0; i < inShape.numIndices(); ++i)
//		{
//			inShape.setIndex(i, inShape.getIndex(i) + vertexCount);
//		}
//	}
//	m_shapes.push_back(std::move(inShape));
//}


//void ShapeGenerator::appendTriangle() 
//{
//	appendShape(makeTriangle());
//}
//void ShapeGenerator::appendPlane(GLuint dimensions = 10) 
//{
//	appendShape(makePlane(dimensions));
//}
//void ShapeGenerator::appendCube() 
//{
//	appendShape(makeCube());
//}
//void ShapeGenerator::appendArrow() {
//	appendShape(makeArrow());
//}
//void ShapeGenerator::appendTube(GLuint resolution = 10, GLfloat radius = 2, GLfloat height = 2)
//{
//	appendShape(makeTube(resolution, radius, height));
//}
//void ShapeGenerator::appendNormals(ShapeData& inShape) {
//	appendShape(makeNormals(inShape));
//}
//void ShapeGenerator::appendNormals()
//{
//	assert(m_shapes.size() > 0);
//	auto m_shapes_size = m_shapes.size();
//	for (auto i = 0; i < m_shapes_size; ++i) 
//	{
//		appendShape(makeNormals(m_shapes.at(i)));
//	}
//}

ShapeData ShapeGenerator::makeTriangle()
{
	ShapeData m_triangle;

	glm::vec3 faceNormal = { 0.0f, 1.0f, 0.0f };

	m_triangle.append_vertices({ glm::vec3(0.0f, 1.0f,  0.0f), glm::vec3(1.0f, 0.0f, 0.0f), faceNormal });
	m_triangle.append_vertices({ glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), faceNormal });
	m_triangle.append_vertices({ glm::vec3(0.5f, 0.0f,  0.0f), glm::vec3(0.0f, 0.0f, 1.0f), faceNormal });

	m_triangle.append_indices(0);
	m_triangle.append_indices(1);
	m_triangle.append_indices(2);

	return m_triangle;
}

ShapeData ShapeGenerator::makePlane(GLuint dimensions = 20) 
{
	ShapeData m_plane;
	GLuint offset = 0;
	for (GLint x = GLint(dimensions * -0.5f); x < GLint(dimensions*0.5f + 1.5); ++x) {
		for (GLint z = GLint(dimensions * -0.5f); z < GLint(dimensions*0.5f + 1.5); ++z) {
			m_plane.append_vertices({ glm::vec3(x, 0, z), randomColor(), glm::vec3(0.0f, 1.0f, 0.0f) });
			//std::cout << "(" << x << "," << z << ")";
		}
	}
	for (GLushort y = 0; y < dimensions; ++y) {
		for (GLushort x = 0; x < dimensions; ++x) {
			GLushort a = x + y + dimensions * y;
			GLushort b = x + y + 1 + dimensions * y;
			GLushort c = x + y + 1 + dimensions + dimensions * y;
			GLushort d = x + y + 2 + dimensions + dimensions * y;
			m_plane.append_indices(c); m_plane.append_indices(a); m_plane.append_indices(b);
			m_plane.append_indices(c); m_plane.append_indices(b); m_plane.append_indices(d);
			//std::cout << c << "," << b << "," << a << "    "
			//		  << c << "," << d << "," << b << "\n";
		}
	}
	return m_plane;
}

ShapeData ShapeGenerator::makeCube() 
{
	ShapeData m_cube;

	// // UP FACE // //
	glm::vec3 faceColor = { 1.0f, 0.5f, 0.0f };
	glm::vec3 faceNormal = { 0.0f, 1.0f, 0.0f };
	m_cube.append_vertices({ glm::vec3(+1.0f, +1.0f, +1.0f), faceColor, faceNormal }); //(2)0
	m_cube.append_vertices({ glm::vec3(-1.0f, +1.0f, +1.0f), faceColor, faceNormal }); //(3)1
	m_cube.append_vertices({ glm::vec3(+1.0f, +1.0f, -1.0f), faceColor, faceNormal }); //(6)2
	m_cube.append_vertices({ glm::vec3(-1.0f, +1.0f, -1.0f), faceColor, faceNormal }); //(7)3
	m_cube.append_indices(3); m_cube.append_indices(0); m_cube.append_indices(2);
	m_cube.append_indices(3); m_cube.append_indices(1); m_cube.append_indices(0);

	// // RIGHT FACE // //
	faceColor = { 0.0f, 1.0f, 0.5f };
	faceNormal = { 1.0f, 0.0f, 0.0f };
	m_cube.append_vertices({ glm::vec3(+1.0f, -1.0f, +1.0f), faceColor, faceNormal }); //(1)4
	m_cube.append_vertices({ glm::vec3(+1.0f, +1.0f, +1.0f), faceColor, faceNormal }); //(2)5
	m_cube.append_vertices({ glm::vec3(+1.0f, +1.0f, -1.0f), faceColor, faceNormal }); //(6)6
	m_cube.append_vertices({ glm::vec3(+1.0f, -1.0f, -1.0f), faceColor, faceNormal }); //(5)7
	m_cube.append_indices(7); m_cube.append_indices(6); m_cube.append_indices(5);
	m_cube.append_indices(7); m_cube.append_indices(5); m_cube.append_indices(4);


	// // FRONT FACE // //
	faceColor = { 0.5f, 0.0f, 1.0f };
	faceNormal = { 0.0f, 0.0f, -1.0f };
	m_cube.append_vertices({ glm::vec3(-1.0f, -1.0f, -1.0f), faceColor, faceNormal }); //(4)8
	m_cube.append_vertices({ glm::vec3(+1.0f, -1.0f, -1.0f), faceColor, faceNormal }); //(5)9
	m_cube.append_vertices({ glm::vec3(+1.0f, +1.0f, -1.0f), faceColor, faceNormal }); //(6)10
	m_cube.append_vertices({ glm::vec3(-1.0f, +1.0f, -1.0f), faceColor, faceNormal }); //(7)11
	m_cube.append_indices(8); m_cube.append_indices(11); m_cube.append_indices(10);
	m_cube.append_indices(8); m_cube.append_indices(10); m_cube.append_indices(9);

	// // LEFT FACE // //
	faceColor = { 0.75f, 0.75f, 0.0f };
	faceNormal = { -1.0f, 0.0f, 0.0f };
	m_cube.append_vertices({ glm::vec3(-1.0f, -1.0f, +1.0f), faceColor, faceNormal }); //(0)12
	m_cube.append_vertices({ glm::vec3(-1.0f, +1.0f, +1.0f), faceColor, faceNormal }); //(3)13
	m_cube.append_vertices({ glm::vec3(-1.0f, -1.0f, -1.0f), faceColor, faceNormal }); //(4)14
	m_cube.append_vertices({ glm::vec3(-1.0f, +1.0f, -1.0f), faceColor, faceNormal }); //(7)15
	m_cube.append_indices(12); m_cube.append_indices(13); m_cube.append_indices(15);
	m_cube.append_indices(12); m_cube.append_indices(15); m_cube.append_indices(14);

	// // BACK FACE // //
	faceColor = { 0.75f, 0.0f, 0.75f };
	faceNormal = { 0.0f, 0.0f, 1.0f };
	m_cube.append_vertices({ glm::vec3(-1.0f, -1.0f, +1.0f), faceColor, faceNormal }); //(0)16
	m_cube.append_vertices({ glm::vec3(+1.0f, -1.0f, +1.0f), faceColor, faceNormal }); //(1)17
	m_cube.append_vertices({ glm::vec3(+1.0f, +1.0f, +1.0f), faceColor, faceNormal }); //(2)18
	m_cube.append_vertices({ glm::vec3(-1.0f, +1.0f, +1.0f), faceColor, faceNormal }); //(3)19
	m_cube.append_indices(17); m_cube.append_indices(18); m_cube.append_indices(19);
	m_cube.append_indices(17); m_cube.append_indices(19); m_cube.append_indices(16);

	// // BOTTOM FACE // //
	faceColor = { 0.0f, 0.75f, 0.75f };
	faceNormal = { 0.0f, -1.0f, 0.0f };
	m_cube.append_vertices({ glm::vec3(-1.0f, -1.0f, +1.0f), faceColor, faceNormal }); //(0)20
	m_cube.append_vertices({ glm::vec3(+1.0f, -1.0f, +1.0f), faceColor, faceNormal }); //(1)21
	m_cube.append_vertices({ glm::vec3(-1.0f, -1.0f, -1.0f), faceColor, faceNormal }); //(4)22
	m_cube.append_vertices({ glm::vec3(+1.0f, -1.0f, -1.0f), faceColor, faceNormal }); //(5)23
	m_cube.append_indices(23); m_cube.append_indices(21); m_cube.append_indices(20);
	m_cube.append_indices(23); m_cube.append_indices(20); m_cube.append_indices(22);

	return m_cube;
}

ShapeData ShapeGenerator::makeArrow() 
{
	ShapeData m_arrow;

	// // UP FACE // // 
	glm::vec3 faceColor = { 1.0f, 0.5f, 0.0f };
	glm::vec3 faceNormal = { 0.0f, 1.0f, 0.0f };
	m_arrow.append_vertices({ glm::vec3(-1.0f, +1.0f, +1.0f), faceColor, faceNormal }); //0
	m_arrow.append_vertices({ glm::vec3(+1.0f, +1.0f, +1.0f), faceColor, faceNormal }); //1
	m_arrow.append_vertices({ glm::vec3(+1.0f, +1.0f, +2.0f), faceColor, faceNormal }); //2
	m_arrow.append_vertices({ glm::vec3(+3.0f, +1.0f, +0.0f), faceColor, faceNormal }); //3
	m_arrow.append_vertices({ glm::vec3(+1.0f, +1.0f, -2.0f), faceColor, faceNormal }); //4
	m_arrow.append_vertices({ glm::vec3(+1.0f, +1.0f, -1.0f), faceColor, faceNormal }); //5
	m_arrow.append_vertices({ glm::vec3(-1.0f, +1.0f, -1.0f), faceColor, faceNormal }); //6

	m_arrow.append_indices(6), m_arrow.append_indices(0), m_arrow.append_indices(1);
	m_arrow.append_indices(6), m_arrow.append_indices(1), m_arrow.append_indices(5);
	m_arrow.append_indices(1), m_arrow.append_indices(2), m_arrow.append_indices(3);
	m_arrow.append_indices(1), m_arrow.append_indices(3), m_arrow.append_indices(5);
	m_arrow.append_indices(5), m_arrow.append_indices(3), m_arrow.append_indices(4);

	// // BOTTOM FACE // // 
	faceColor = { 0.0f, 1.0f, 0.5f };
	faceNormal = { 0.0f, -1.0f, 0.0f };
	m_arrow.append_vertices({ glm::vec3(-1.0f, -1.0f, +1.0f), faceColor, faceNormal }); //7
	m_arrow.append_vertices({ glm::vec3(+1.0f, -1.0f, +1.0f), faceColor, faceNormal }); //8
	m_arrow.append_vertices({ glm::vec3(+1.0f, -1.0f, +2.0f), faceColor, faceNormal }); //9
	m_arrow.append_vertices({ glm::vec3(+3.0f, -1.0f, +0.0f), faceColor, faceNormal }); //10
	m_arrow.append_vertices({ glm::vec3(+1.0f, -1.0f, -2.0f), faceColor, faceNormal }); //11
	m_arrow.append_vertices({ glm::vec3(+1.0f, -1.0f, -1.0f), faceColor, faceNormal }); //12
	m_arrow.append_vertices({ glm::vec3(-1.0f, -1.0f, -1.0f), faceColor, faceNormal }); //13

	m_arrow.append_indices(10), m_arrow.append_indices(9), m_arrow.append_indices(8);
	m_arrow.append_indices(10), m_arrow.append_indices(8), m_arrow.append_indices(12);
	m_arrow.append_indices(10), m_arrow.append_indices(12), m_arrow.append_indices(11);
	m_arrow.append_indices(12), m_arrow.append_indices(8), m_arrow.append_indices(7);
	m_arrow.append_indices(12), m_arrow.append_indices(7), m_arrow.append_indices(13);

	// // BACK FACES // //
	faceColor = { 0.5f, 0.0f, 1.0f };
	faceNormal = { -1.0f, 0.0f, 0.0f };
	m_arrow.append_vertices({ glm::vec3(+1.0f, +1.0f, +1.0f), faceColor, faceNormal }); //(1)14
	m_arrow.append_vertices({ glm::vec3(+1.0f, +1.0f, +2.0f), faceColor, faceNormal }); //(2)15
	m_arrow.append_vertices({ glm::vec3(+1.0f, -1.0f, +1.0f), faceColor, faceNormal }); //(8)16
	m_arrow.append_vertices({ glm::vec3(+1.0f, -1.0f, +2.0f), faceColor, faceNormal }); //(9)17

	m_arrow.append_vertices({ glm::vec3(-1.0f, +1.0f, +1.0f), faceColor, faceNormal }); //(0)18
	m_arrow.append_vertices({ glm::vec3(-1.0f, +1.0f, -1.0f), faceColor, faceNormal }); //(6)19
	m_arrow.append_vertices({ glm::vec3(-1.0f, -1.0f, +1.0f), faceColor, faceNormal }); //(7)20
	m_arrow.append_vertices({ glm::vec3(-1.0f, -1.0f, -1.0f), faceColor, faceNormal }); //(13)21

	m_arrow.append_vertices({ glm::vec3(+1.0f, +1.0f, -2.0f), faceColor, faceNormal }); //(4)22
	m_arrow.append_vertices({ glm::vec3(+1.0f, +1.0f, -1.0f), faceColor, faceNormal }); //(5)23
	m_arrow.append_vertices({ glm::vec3(+1.0f, -1.0f, -2.0f), faceColor, faceNormal }); //(11)24
	m_arrow.append_vertices({ glm::vec3(+1.0f, -1.0f, -1.0f), faceColor, faceNormal }); //(12)25

	m_arrow.append_indices(16), m_arrow.append_indices(17), m_arrow.append_indices(15);
	m_arrow.append_indices(16), m_arrow.append_indices(15), m_arrow.append_indices(14);

	m_arrow.append_indices(21), m_arrow.append_indices(20), m_arrow.append_indices(18);
	m_arrow.append_indices(21), m_arrow.append_indices(18), m_arrow.append_indices(19);

	m_arrow.append_indices(24), m_arrow.append_indices(25), m_arrow.append_indices(23);
	m_arrow.append_indices(24), m_arrow.append_indices(23), m_arrow.append_indices(22);

	// // LEFT FACE // //
	faceColor = { 0.5f, 1.0f, 0.0f };
	faceNormal = { 0.0f, 0.0f, -1.0f };
	m_arrow.append_vertices({ glm::vec3(+1.0f, +1.0f, -1.0f), faceColor, faceNormal }); //(5)26
	m_arrow.append_vertices({ glm::vec3(-1.0f, +1.0f, -1.0f), faceColor, faceNormal }); //(6)27
	m_arrow.append_vertices({ glm::vec3(+1.0f, -1.0f, -1.0f), faceColor, faceNormal }); //(12)28
	m_arrow.append_vertices({ glm::vec3(-1.0f, -1.0f, -1.0f), faceColor, faceNormal }); //(13)29

	m_arrow.append_indices(29), m_arrow.append_indices(27), m_arrow.append_indices(26);
	m_arrow.append_indices(29), m_arrow.append_indices(26), m_arrow.append_indices(28);

	// // RIGHT FACE // //
	faceColor = { 0.0f, 0.5f, 1.0f };
	faceNormal = { 0.0f, 0.0f, +1.0f };
	m_arrow.append_vertices({ glm::vec3(-1.0f, +1.0f, +1.0f), faceColor, faceNormal }); //(0)30
	m_arrow.append_vertices({ glm::vec3(+1.0f, +1.0f, +1.0f), faceColor, faceNormal }); //(1)31
	m_arrow.append_vertices({ glm::vec3(-1.0f, -1.0f, +1.0f), faceColor, faceNormal }); //(7)32
	m_arrow.append_vertices({ glm::vec3(+1.0f, -1.0f, +1.0f), faceColor, faceNormal }); //(8)33

	m_arrow.append_indices(30), m_arrow.append_indices(32), m_arrow.append_indices(33);
	m_arrow.append_indices(30), m_arrow.append_indices(33), m_arrow.append_indices(31);

	// // LEFT DIAGONAL FACE // //
	faceColor = { 0.75f, 0.75f, 0.0f };
	faceNormal = glm::normalize(glm::cross(
		glm::vec3(+3.0f, -1.0f, +0.0f) - glm::vec3(+1.0f, -1.0f, -2.0f),
		glm::vec3(+3.0f, -1.0f, +0.0f) - glm::vec3(+3.0f, +1.0f, +0.0f)));

	m_arrow.append_vertices({ glm::vec3(+3.0f, +1.0f, +0.0f), faceColor, faceNormal }); //(3)34
	m_arrow.append_vertices({ glm::vec3(+1.0f, +1.0f, -2.0f), faceColor, faceNormal }); //(4)35
	m_arrow.append_vertices({ glm::vec3(+3.0f, -1.0f, +0.0f), faceColor, faceNormal }); //(10)36
	m_arrow.append_vertices({ glm::vec3(+1.0f, -1.0f, -2.0f), faceColor, faceNormal }); //(11)37

	m_arrow.append_indices(37), m_arrow.append_indices(35), m_arrow.append_indices(34);
	m_arrow.append_indices(37), m_arrow.append_indices(34), m_arrow.append_indices(36);

	// // RIGHT DIAGONAL FACE // //
	faceColor = { 0.75f, 0.0f, 0.75f };
	faceNormal = glm::normalize(glm::cross(
		glm::vec3(+1.0f, -1.0f, +2.0f) - glm::vec3(+3.0f, -1.0f, +0.0f),
		glm::vec3(+1.0f, -1.0f, +2.0f) - glm::vec3(+1.0f, +1.0f, +2.0f)));

	m_arrow.append_vertices({ glm::vec3(+1.0f, +1.0f, +2.0f), faceColor, faceNormal }); //(2)38
	m_arrow.append_vertices({ glm::vec3(+3.0f, +1.0f, +0.0f), faceColor, faceNormal }); //(3)39
	m_arrow.append_vertices({ glm::vec3(+1.0f, -1.0f, +2.0f), faceColor, faceNormal }); //(9)40
	m_arrow.append_vertices({ glm::vec3(+3.0f, -1.0f, +0.0f), faceColor, faceNormal }); //(10)41

	m_arrow.append_indices(38), m_arrow.append_indices(40), m_arrow.append_indices(41);
	m_arrow.append_indices(38), m_arrow.append_indices(41), m_arrow.append_indices(39);

	return m_arrow;
}

ShapeData ShapeGenerator::makeTube(GLuint resolution = 10, GLfloat radius = 2, GLfloat height = 2) 
{
	ShapeData outTube = makePlane(resolution);
	GLuint width = resolution + 1;
	GLfloat y = -height * 0.5f;
	GLfloat x = 0.0f;
	GLfloat z = 0.0f;
	GLfloat y_step = height / width;
	GLint vertex = 0;
	GLfloat angle = 0.0f;
	GLfloat angleStep = (2 * float(PI)) / resolution;

	for (GLuint i = width; i > 0; --i, y += y_step) {

		for (GLuint j = 0; j < width; ++j, ++vertex, angle -= angleStep) {
			x = radius * cos(angle);
			z = radius * sin(angle);
			outTube.setVertex<0>(vertex, { x, y, z });
			outTube.setVertex<2>(vertex, { cos(angle), 0, sin(angle) });
		}
	}
	return outTube;
}

ShapeData ShapeGenerator::makeNormals(ShapeData& inShape) 
{
	ShapeData m_normals;
	GLint j = 0;
	for (GLint i = 0; i < inShape.numVertices(); ++i) {
		m_normals.append_vertices(
		{
			inShape.getVertex<inShape.position>(i),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		});
		m_normals.append_vertices(
		{
			inShape.getVertex<inShape.position>(i) + inShape.getVertex<inShape.normal>(i),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f)
		});
		m_normals.append_indices(j);
		++j;
		m_normals.append_indices(j);
		++j;
	}
	return m_normals;
}


glm::vec3 ShapeGenerator::randomColor()
{
	return glm::vec3(
		rand() / (float)RAND_MAX,
		rand() / (float)RAND_MAX,
		rand() / (float)RAND_MAX
	);
}