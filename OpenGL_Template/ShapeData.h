#pragma once
#include "Vertex.h"
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <tuple>
#include <cassert>


struct ShapeData {
	
	// // ----- Type Definitions ----- // //
	typedef std::tuple<glm::vec3, glm::vec3, glm::vec3> shapeType;
	typedef std::vector<shapeType>						vertexType;
	typedef std::vector<GLushort>						indexType;

	// // ----- Attribute Enumeration ----- // //
	enum attr { position = 0, color = 1, normal = 2 };

	// // ----- Big 6 ----- // //
	ShapeData();
	ShapeData(const vertexType s_vertices, const indexType s_indices);
	~ShapeData() {};
	ShapeData(const ShapeData&) = delete;
	ShapeData(const ShapeData&& other);
	ShapeData& operator = (const ShapeData&) = delete;
	ShapeData& operator = (ShapeData&& other);

	// // ----- Addition Assign ----- // //
	ShapeData& operator += (ShapeData& other);

	// // ----- Append ----- // //
	void append_vertices(const shapeType s_shape);
	void append_indices(const GLushort s_index);

	// // ----- Setters ----- // //
	//void setVertex(std::size_t loc, const shapeType& data);

	template <std::size_t attr>
	void setVertex(std::size_t loc, const glm::vec3& data);

	void setVertex(std::size_t loc, const shapeType& data)
	{
		assert(m_num_vertices >= loc);
		m_vertices.at(loc) = data;
	}
	//template <std::size_t attr>
	//void setVertex(std::size_t loc, const glm::vec3& data)
	//{
	//	assert(m_num_vertices >= loc);
	//	std::get<attr>(m_vertices.at(loc)) = data;
	//}

	// // ----- Getters ----- // //
	shapeType getVertex(std::size_t i);
	template <std::size_t attr>
	glm::vec3 getVertex(const std::size_t i);

	std::size_t numIndices()	{ return m_num_indices; }
	std::size_t numVertices()	{ return m_num_vertices; }
	vertexType vertices()		{ return m_vertices; }
	indexType indices()			{ return m_indices; }

	// // ----- Size Getters ----- // //
	GLsizeiptr sizeVertices()	{ return m_vertices.size() * sizeof(Vertex); }
	GLsizeiptr sizeIndices()	{ return m_indices.size() * sizeof(GLushort); }
	GLsizeiptr sizeShape()		{ return sizeVertices() + sizeIndices(); }

private:
	// // ----- Member Variables ----- // //
	vertexType	m_vertices;
	indexType	m_indices;
	std::size_t m_num_vertices;
	std::size_t m_num_indices;
};