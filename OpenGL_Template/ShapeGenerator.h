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

		m_triangle.indeces.push_back(0);
		m_triangle.indeces.push_back(1);
		m_triangle.indeces.push_back(2);

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
		m_cube.indeces.push_back(4); m_cube.indeces.push_back(7); m_cube.indeces.push_back(6); 
		m_cube.indeces.push_back(4); m_cube.indeces.push_back(6); m_cube.indeces.push_back(5);

		// // Top
		m_cube.indeces.push_back(7); m_cube.indeces.push_back(6); m_cube.indeces.push_back(2);
		m_cube.indeces.push_back(7); m_cube.indeces.push_back(2); m_cube.indeces.push_back(3);

		// // Right
		m_cube.indeces.push_back(5); m_cube.indeces.push_back(1); m_cube.indeces.push_back(2);
		m_cube.indeces.push_back(5); m_cube.indeces.push_back(2); m_cube.indeces.push_back(6);

		// // Back
		m_cube.indeces.push_back(1); m_cube.indeces.push_back(0); m_cube.indeces.push_back(3);
		m_cube.indeces.push_back(1); m_cube.indeces.push_back(3); m_cube.indeces.push_back(2);

		// // Left
		m_cube.indeces.push_back(0); m_cube.indeces.push_back(4); m_cube.indeces.push_back(7);
		m_cube.indeces.push_back(0); m_cube.indeces.push_back(7); m_cube.indeces.push_back(3);

		// // Bottom
		m_cube.indeces.push_back(5); m_cube.indeces.push_back(4); m_cube.indeces.push_back(0);
		m_cube.indeces.push_back(5); m_cube.indeces.push_back(0); m_cube.indeces.push_back(1);

		


		return m_cube;
	}

}

