#pragma once
#include "Vertex.h"
#include <GL\glew.h>
#include <vector>
#include <tuple>


struct ShapeData {
	typedef std::tuple<glm::vec3, glm::vec3, glm::vec3> shapeType;

	// // ----- Member Variables ----- // //
	typedef std::vector<shapeType> vertexType;
	vertexType vertices;
	std::vector<GLushort> indices;


	// // ----- Constructor ----- // //
	ShapeData() :
		vertices(shapeType),
		indices(NULL)
	{};

	ShapeData(const vertexType s_vertices, const std::vector<GLushort> s_indeces) :
		vertices(s_vertices),
		indices(s_indeces)
	{};

	// // ----- Size Getters ----- // //
	GLsizeiptr sizeVertices() {
		return vertices.size() * sizeof(Vertex);
	}
	GLsizeiptr sizeIndices() {
		return indices.size() * sizeof(GLushort);
	}
	GLsizeiptr sizeShape() {
		return 
			vertices.size() * sizeof(Vertex) + 
			indices.size() * sizeof(GLushort);
	}
	
	GLuint numIndices() {
		return GLuint(indices.size());
	}
};