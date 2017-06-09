#pragma once
#include "Vertex.h"
#include <GL\glew.h>
#include <vector>

struct ShapeData {

	// // ----- Member Variables ----- // //
	std::vector<Vertex> vertices;
	std::vector<GLushort> indices;


	// // ----- Constructor ----- // //
	ShapeData() :
		vertices(), 
		indices(NULL)
	{};
	ShapeData(const std::vector<Vertex> s_vertices, const std::vector<GLushort> s_indeces) :
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
	GLuint numIndices() {
		return GLuint(indices.size());
	}
};