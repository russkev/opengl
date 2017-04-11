#pragma once
#include "Vertex.h"
#include <GL\glew.h>

struct ShapeData {

	// // ----- Member Variables ----- // //
	Vertex* vertices;
	GLuint numVertices;
	GLushort* indeces;
	GLuint numIndeces;

	// // ----- Constructor ----- // //
	ShapeData() :
		vertices(0), numVertices(0), indeces(0), numIndeces(0) {};

	// // ----- Size Getters ----- // //
	GLsizeiptr sizeVertices() {
		return numVertices * sizeof(Vertex);
	}
	GLsizeiptr sizeIndeces() {
		return numIndeces * sizeof(GLushort);
	}

	// // ----- Cleanup ----- //
	void cleanup() {
		delete[] vertices;
		delete[] indeces;
		numVertices = numIndeces = 0;
	}
};