#pragma once
#include "Vertex.h"
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <tuple>
#include <cassert>
#include <string>


struct ShapeData 
{
public:	
	// // ----- Type Definitions ----- // //
	typedef std::tuple<glm::vec3, glm::vec3, glm::vec3>			vertexDataType;
	typedef std::tuple<glm::vec3, glm::vec3, glm::vec3, GLuint>	vertexType;
	typedef std::vector<vertexType>								verticesType;
	typedef GLushort											indexType;
	typedef std::vector<indexType>								indicesType;
	typedef std::vector<vertexType>::const_iterator				vrt_iterator;
	typedef std::vector<GLushort>::const_iterator				ind_iterator;


	// // ----- Attribute Enumeration ----- // //
	enum attr { position = 0, color = 1, normal = 2, id = 3 };

	// // ----- Big 6 ----- // //
	ShapeData();
	ShapeData(const verticesType s_vertices, const indicesType s_indices);
	~ShapeData() {};
	ShapeData(const ShapeData&) = delete;
	ShapeData(const ShapeData&& other);
	ShapeData& operator = (const ShapeData&) = delete;
	ShapeData& operator = (ShapeData&& other);

	// // ----- Addition Assign ----- // //
	ShapeData& operator += (ShapeData& other);

	// // ----- Append ----- // //
	void append_vertices(const vertexDataType s_shape);
	void append_indices(const GLushort s_index);

	// // ----- Setters ----- // //
	void setVertex(std::size_t loc, const vertexType& data);
	template <std::size_t attr>
	void setVertex(std::size_t loc, const glm::vec3& data)
	{
		assert(m_num_vertices >= loc);
		std::get<attr>(m_vertices.at(loc)) = data;
	}
	void setIndex(std::size_t loc, const indexType& data);
	void setId(GLuint s_id);

	// // ----- Transform ----- // //
	void transform(glm::mat4 transformMatrix);
	void transform(verticesType& inVertices, const glm::mat4 transformMatrix);

	// // ----- IDs ----- // //
	void updateIds();

	// // ----- Getters ----- // //
	vertexType getVertex(std::size_t i);
	template <std::size_t attr>
	glm::vec3 getVertex(const std::size_t i)
	{
		assert(m_num_vertices > 0);
		return std::get<attr>(m_vertices.at(i));
	}
	indexType getIndex(std::size_t i);
	std::size_t numIndices()	{ return m_num_indices; }
	std::size_t numVertices()	{ return m_num_vertices; }
	verticesType vertices()		{ return m_vertices; }
	indicesType indices()		{ return m_indices; }

	// // ----- Size Getters ----- // //
	GLsizeiptr sizeVertices()	{ return m_vertices.size() * sizeof(Vertex); }
	GLsizeiptr sizeIndices()	{ return m_indices.size()  * sizeof(GLushort); }
	GLsizeiptr sizeShape()		{ return sizeVertices()    + sizeIndices(); }

	// // ----- Iterators ----- // //
	vrt_iterator vert_begin()	{ return m_vertices.begin(); }
	vrt_iterator vert_end()		{ return m_vertices.end(); }
	ind_iterator indx_begin()	{ return m_indices.begin(); }
	ind_iterator indx_end()		{ return m_indices.end(); }
	

private:
	// // ----- Member Variables ----- // //
	verticesType	m_vertices;
	indicesType		m_indices;
	std::size_t		m_num_vertices;
	std::size_t		m_num_indices;
	GLuint			m_id;
};