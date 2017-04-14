#pragma once
#include "Vertex.h"
#include <GL\glew.h>
#include <memory>

struct ShapeData {

	// // ----- Member Variables ----- // //
	std::unique_ptr<Vertex[]> vertices;
	GLuint numVertices;
	GLushort* indeces;
	GLuint numIndeces;

	// // ----- Constructor ----- // //
	ShapeData() :
		vertices(nullptr), 
		numVertices(0), 
		indeces(0), 
		numIndeces(0) 
	{};

	// // ----- Size Getters ----- // //
	GLsizeiptr sizeVertices() {
		return numVertices * sizeof(Vertex);
	}
	GLsizeiptr sizeIndeces() {
		return numIndeces * sizeof(GLushort);
	}

	// // ----- Cleanup ----- //
	void cleanup() {
		delete[] indeces;
		numVertices = numIndeces = 0;
	}
};