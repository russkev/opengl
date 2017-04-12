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
	ShapeData(GLuint s_numVertices, Vertex s_vertex, GLuint s_numIndeces) :
		//vertices(std::make_unique<Vertex[]>(s_vertex)), 
		numVertices(s_numVertices), 
		indeces(0), 
		numIndeces(s_numIndeces) 
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