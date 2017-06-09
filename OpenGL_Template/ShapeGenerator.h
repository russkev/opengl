#pragma once

#include <glm/matrix.hpp>
#include <math.h>

#include "ShapeData.h"
#include "Vertex.h"


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
		m_cube.indices.push_back(7); m_cube.indices.push_back(6); m_cube.indices.push_back(2);
		m_cube.indices.push_back(7); m_cube.indices.push_back(2); m_cube.indices.push_back(3);
		// // Right
		m_cube.indices.push_back(5); m_cube.indices.push_back(1); m_cube.indices.push_back(2);
		m_cube.indices.push_back(5); m_cube.indices.push_back(2); m_cube.indices.push_back(6);
		// // Back
		m_cube.indices.push_back(1); m_cube.indices.push_back(0); m_cube.indices.push_back(3);
		m_cube.indices.push_back(1); m_cube.indices.push_back(3); m_cube.indices.push_back(2);
		// // Left
		m_cube.indices.push_back(0); m_cube.indices.push_back(4); m_cube.indices.push_back(7);
		m_cube.indices.push_back(0); m_cube.indices.push_back(7); m_cube.indices.push_back(3);
		// // Bottom
		m_cube.indices.push_back(5); m_cube.indices.push_back(4); m_cube.indices.push_back(0);
		m_cube.indices.push_back(5); m_cube.indices.push_back(0); m_cube.indices.push_back(1);
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
			6, 1, 0, 6, 5, 1, 1, 3, 2, 1, 5, 3, 5, 4, 3, // Top face
			0, 8, 7, 0, 1, 8,
			8, 1, 2, 8, 2, 9,
			11, 4, 5, 11, 5, 12,
			13, 12, 5, 13, 5, 6,
			11, 10, 3, 11, 3, 4,
			13, 6, 0, 13, 0, 7,
			3, 10, 9, 3, 9, 2,
			10, 8, 9, 10, 12, 8, 10, 11, 12, 12, 7, 8, 12, 13, 7 // Bottom Face
		};

		for (int i = 0; i < (sizeof(tempTriangles) / sizeof(GLushort)); ++i) {
			m_arrow.indices.push_back(tempTriangles[i]);
		}
		return m_arrow;
	}
}

