#pragma once
#include "Vertex.h"
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <tuple>


struct ShapeData {
	
	// // ----- Type Definitions ----- // //
	typedef std::tuple<glm::vec3, glm::vec3, glm::vec3> shapeType;
	typedef std::vector<shapeType>						vertexType;
	typedef std::vector<GLushort>						indexType;

	// // ----- Member Variables ----- // //
	vertexType	vertices;
	indexType	indices;
	std::size_t m_num_vertices;
	std::size_t m_num_indices;


	// // ----- Constructor ----- // //
	ShapeData() :
		vertices(vertexType{}),
		indices(indexType{}),
		m_num_vertices(0),
		m_num_indices(0)
	{};

	// // ----- Constructor ----- // //
	ShapeData(const vertexType s_vertices, const indexType s_indices) :
		vertices(s_vertices),
		indices(s_indices),
		m_num_vertices(m_num_vertices + s_vertices.size()),
		m_num_indices(m_num_indices + s_indices.size())
	{};

	// // ----- PUSH ----- // //
	void append_vertices(const shapeType s_shape) {
		vertices.push_back(s_shape);
		m_num_vertices++;
	}

	void append_indices(const GLushort s_index) {
		indices.push_back(s_index);
		m_num_indices++;
	}


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