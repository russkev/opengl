#pragma once
#include "Vertex.h"
#include <GL\glew.h>
#include <vector>

struct ShapeData {

	// // ----- Member Variables ----- // //
	std::vector<Vertex> vertices;
	std::vector<GLushort> indeces;


	// // ----- Constructor ----- // //
	ShapeData() :
		vertices(), 
		indeces(NULL)
	{};
	ShapeData(const std::vector<Vertex> s_vertices, const std::vector<GLushort> s_indeces) :
		vertices(s_vertices),
		indeces(s_indeces)
	{};

	// // ----- Size Getters ----- // //
	GLsizeiptr sizeVertices() {
		return vertices.size() * sizeof(Vertex);
	}
	GLsizeiptr sizeIndeces() {
		return indeces.size() * sizeof(GLushort);
	}
	GLuint numIndeces() {
		return GLuint(indeces.size());
	}
};