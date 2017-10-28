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
	vertexType	m_vertices;
	indexType	m_indices;
	std::size_t m_num_vertices;
	std::size_t m_num_indices;


	// // ----- Constructor ----- // //
	ShapeData() :
		m_vertices(vertexType{}),
		m_indices(indexType{}),
		m_num_vertices(0u),
		m_num_indices(0u)
	{};

	// // ----- Constructor ----- // //
	ShapeData(const vertexType s_vertices, const indexType s_indices) :
		m_vertices(s_vertices),
		m_indices(s_indices),
		m_num_vertices(m_num_vertices + s_vertices.size()),
		m_num_indices(m_num_indices + s_indices.size())
	{};

	// // ----- Destructor ----- // //
	~ShapeData() {};

	// // ----- Copy Constructor ----- // //
	ShapeData(const ShapeData&) = delete;

	// // ----- Move Constructor ----- // //
	ShapeData(const ShapeData&& other) :
		m_vertices		(std::move(other.m_vertices		)),
		m_indices		(std::move(other.m_indices		)),
		m_num_vertices	(std::move(other.m_num_vertices	)),
		m_num_indices	(std::move(other.m_num_indices	))
	{};

	// // ----- Copy Assign ----- // //
	ShapeData& operator = (const ShapeData&) = delete;
	
	// // ----- Move Assign ----- // //
	ShapeData& operator = (ShapeData&& other) 
	{
		(*this).~ShapeData();
		return *new (this) ShapeData(std::move(other));
	}

	// // ----- Addition Assign ----- // //
	ShapeData& operator += (ShapeData& other) 
	{
		//auto temp_num_indices = m_num_vertices;
		for (std::size_t i = 0u; i < other.numIndices(); ++i) 
		{
			m_indices.push_back(other.m_indices.at(i) + (GLushort)m_num_vertices);
		}
		m_vertices.insert(m_vertices.end(), other.m_vertices.begin(), other.m_vertices.end());
		m_num_indices += other.m_num_indices;
		m_num_vertices += other.m_num_vertices;
		return *this;
	}

	// // ----- PUSH ----- // //
	void append_vertices(const shapeType s_shape) {
		m_vertices.push_back(s_shape);
		m_num_vertices++;
	}

	void append_indices(const GLushort s_index) {
		m_indices.push_back(s_index);
		m_num_indices++;
	}


	// // ----- Size Getters ----- // //
	GLsizeiptr sizeVertices() {
		return m_vertices.size() * sizeof(Vertex);
	}
	GLsizeiptr sizeIndices() {
		return m_indices.size() * sizeof(GLushort);
	}
	GLsizeiptr sizeShape() {
		return 
			m_vertices.size() * sizeof(Vertex) + 
			m_indices.size() * sizeof(GLushort);
	}
	
	GLuint numIndices() {
		return GLuint(m_indices.size());
	}
};