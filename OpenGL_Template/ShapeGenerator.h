#pragma once

#include <glm/matrix.hpp>
#include <math.h>

#include "ShapeData.h"
#include "Vertex.h"

glm::vec3 randomColor() {
	return glm::vec3(
		rand() / (float)RAND_MAX,
		rand() / (float)RAND_MAX,
		rand() / (float)RAND_MAX
		);
}

namespace ShapeGenerator {
	ShapeData makeTriangle() {
		ShapeData m_triangle;

		m_triangle.vertices.push_back({ glm::vec3(0.0f, 1.0f,  0.0f), glm::vec3(1.0f, 0.0f, 0.0f) });
		m_triangle.vertices.push_back({ glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) });
		m_triangle.vertices.push_back({ glm::vec3(0.5f, 0.0f,  0.0f), glm::vec3(0.0f, 0.0f, 1.0f) });

		m_triangle.indices.push_back(0);
		m_triangle.indices.push_back(1);
		m_triangle.indices.push_back(2);

		return m_triangle;
	}
	ShapeData makePlane(GLuint dimensions = 2) {
		ShapeData m_plane;
		GLuint offset = 0;
		for (GLint x = GLint(dimensions * -0.5f); x < GLint(dimensions*0.5f+1.5); ++x){
			for (GLint z = GLint(dimensions * -0.5f); z < GLint(dimensions*0.5f + 1.5); ++z) {
				m_plane.vertices.push_back({ glm::vec3(x, 0, z), randomColor(), glm::vec3(0.0f, 1.0f, 0.0f) });
				//std::cout << "(" << x << "," << z << ")";
			}
		}
		for (GLushort y = 0; y < dimensions; ++y) {
			for (GLushort x = 0; x < dimensions; ++x) {
				GLushort a = x + y + dimensions * y;
				GLushort b = x + y + 1 + dimensions * y;
				GLushort c = x + y + 1 + dimensions + dimensions * y;
				GLushort d = x + y + 2 + dimensions + dimensions * y;
				m_plane.indices.push_back(c); m_plane.indices.push_back(a); m_plane.indices.push_back(b);
				m_plane.indices.push_back(c); m_plane.indices.push_back(b); m_plane.indices.push_back(d);
				//std::cout << c << "," << b << "," << a << "    "
				//		  << c << "," << d << "," << b << "\n";
			}
		}




		return m_plane;
	}
	ShapeData makeCube() {
		ShapeData m_cube;

		m_cube.vertices.push_back({ glm::vec3(-1.0f, -1.0f, +1.0f) }); //0
		m_cube.vertices.push_back({ glm::vec3(+1.0f, -1.0f, +1.0f) }); //1
		m_cube.vertices.push_back({ glm::vec3(+1.0f, +1.0f, +1.0f) }); //2
		m_cube.vertices.push_back({ glm::vec3(-1.0f, +1.0f, +1.0f) }); //3
		m_cube.vertices.push_back({ glm::vec3(-1.0f, -1.0f, -1.0f) }); //4
		m_cube.vertices.push_back({ glm::vec3(+1.0f, -1.0f, -1.0f) }); //5
		m_cube.vertices.push_back({ glm::vec3(+1.0f, +1.0f, -1.0f) }); //6
		m_cube.vertices.push_back({ glm::vec3(-1.0f, +1.0f, -1.0f) }); //7

		// // Set colours to relate to vertices so that -1 in x is 0 in r, 1 in x is 1 in r, etc.
		for (int i = 0; i < m_cube.vertices.size(); ++i) {
			for (int j = 0; j < 3; ++j) {
				float temp = (m_cube.vertices.at(i).position[j]+1)/2;
				m_cube.vertices.at(i).color[j] = (m_cube.vertices.at(i).position[j] + 1) / 2;
			}
		}
		// // Front
		m_cube.indices.push_back(4); m_cube.indices.push_back(7); m_cube.indices.push_back(6); 
		m_cube.indices.push_back(4); m_cube.indices.push_back(6); m_cube.indices.push_back(5);
		// // Top
		m_cube.indices.push_back(7); m_cube.indices.push_back(2); m_cube.indices.push_back(6);
		m_cube.indices.push_back(7); m_cube.indices.push_back(3); m_cube.indices.push_back(2);
		// // Right
		m_cube.indices.push_back(5); m_cube.indices.push_back(2); m_cube.indices.push_back(1);
		m_cube.indices.push_back(5); m_cube.indices.push_back(6); m_cube.indices.push_back(2);
		// // Back
		m_cube.indices.push_back(1); m_cube.indices.push_back(3); m_cube.indices.push_back(0);
		m_cube.indices.push_back(1); m_cube.indices.push_back(2); m_cube.indices.push_back(3);
		// // Left
		m_cube.indices.push_back(0); m_cube.indices.push_back(7); m_cube.indices.push_back(4);
		m_cube.indices.push_back(0); m_cube.indices.push_back(3); m_cube.indices.push_back(7);
		// // Bottom
		m_cube.indices.push_back(5); m_cube.indices.push_back(0); m_cube.indices.push_back(4);
		m_cube.indices.push_back(5); m_cube.indices.push_back(1); m_cube.indices.push_back(0);
		return m_cube;
	}
	ShapeData makeArrow() {
		ShapeData m_arrow;
		m_arrow.vertices.push_back({ glm::vec3(-1.0f, +1.0f, +1.0f) }); //0
		m_arrow.vertices.push_back({ glm::vec3(+1.0f, +1.0f, +1.0f) }); //1
		m_arrow.vertices.push_back({ glm::vec3(+1.0f, +1.0f, +2.0f) }); //2
		m_arrow.vertices.push_back({ glm::vec3(+3.0f, +1.0f, +0.0f) }); //3
		m_arrow.vertices.push_back({ glm::vec3(+1.0f, +1.0f, -2.0f) }); //4
		m_arrow.vertices.push_back({ glm::vec3(+1.0f, +1.0f, -1.0f) }); //5
		m_arrow.vertices.push_back({ glm::vec3(-1.0f, +1.0f, -1.0f) }); //6

		m_arrow.vertices.push_back({ glm::vec3(-1.0f, -1.0f, +1.0f) }); //7
		m_arrow.vertices.push_back({ glm::vec3(+1.0f, -1.0f, +1.0f) }); //8
		m_arrow.vertices.push_back({ glm::vec3(+1.0f, -1.0f, +2.0f) }); //9
		m_arrow.vertices.push_back({ glm::vec3(+3.0f, -1.0f, +0.0f) }); //10
		m_arrow.vertices.push_back({ glm::vec3(+1.0f, -1.0f, -2.0f) }); //11
		m_arrow.vertices.push_back({ glm::vec3(+1.0f, -1.0f, -1.0f) }); //12
		m_arrow.vertices.push_back({ glm::vec3(-1.0f, -1.0f, -1.0f) }); //13

		// // Set colours to relate to vertices so that -1 in x is 0 in r, 1 in x is 1 in r, etc.
		for (int i = 0; i < m_arrow.vertices.size(); ++i) {
			for (int j = 0; j < 3; ++j) {
				float temp = fmod((m_arrow.vertices.at(i).position[j] + 1) / 2, 1.01f);
				m_arrow.vertices.at(i).color[j] = (m_arrow.vertices.at(i).position[j] + 1) / 2;
			}
		}

		GLushort tempTriangles[] = {
			6, 0, 1, 6, 1, 5, 1, 2, 3, 1, 3, 5, 5, 3, 4, // Top face
			0, 7, 8, 0, 8, 1,
			8, 2, 1, 8, 9, 2,
			11, 5, 4, 11, 12, 5,
			13, 5, 12, 13, 6, 5,
			11, 3, 10, 11, 4, 3,
			13, 0, 6, 13, 7, 0,
			3, 9, 10, 3, 2, 9,
			10, 9, 8, 10, 8, 12, 10, 12, 11, 12, 8, 7, 12, 7, 13 // Bottom Face
		};

		for (int i = 0; i < (sizeof(tempTriangles) / sizeof(GLushort)); ++i) {
			m_arrow.indices.push_back(tempTriangles[i]);
		}
		return m_arrow;
	}
	ShapeData makeNormals(ShapeData inShape) {
		ShapeData m_normals;
		
		GLint j = 0;
		for (GLint i = 0; i < inShape.vertices.size(); ++i) {
			m_normals.vertices.push_back(Vertex(inShape.vertices.at(i).position, glm::vec3(1.0f, 0.0f, 0.0f)));
			m_normals.vertices.push_back(Vertex(inShape.vertices.at(i).position + inShape.vertices.at(i).normal, glm::vec3(1.0f, 0.0f, 0.0f)));
			m_normals.indices.push_back(j);
			++j;
			m_normals.indices.push_back(j);
			++j;
		}
		return m_normals;
	}
}

