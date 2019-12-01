#pragma once
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <tuple>
#include <cassert>
#include <string>
#include <map>
#include <cstdlib>

#include "../VectorUtils.h"
#include "Vertex.h"

struct Mesh 
{
public:
	
	// // ----- Type Definitions ----- // //
	//                  Position    Colour     Normal      UV
	// typedef std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2>								vertexDataType;

	// --- Don't forget to update the gl_introspect_tuple call when changing this --- //
	//                  Position,   Colour,    Normal,      UV,     ID,    Tangent,  Bitangent
	/*typedef std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint, glm::vec3, glm::vec3>	vertexType;*/
	typedef std::vector<Vertex>						verticesType;
	typedef GLushort								indexType;
	typedef std::vector<indexType>					indicesType;
	typedef std::vector<Vertex>::const_iterator		vrt_iterator;
	typedef std::vector<GLushort>::const_iterator	ind_iterator;

	// // ----- Attribute Enumeration ----- // //
	//enum attr { position = 0, color = 1, normal = 2, uv = 3, id = 4, tangent = 5, bitangent = 6 };

	// // ----- Big 6 ----- // //
	Mesh();
	Mesh(const verticesType s_vertices, const indicesType s_indices);
	//~Mesh() {};
	//Mesh(const Mesh&) = delete;
	Mesh(const Mesh&& other);
	//Mesh& operator = (const Mesh&) = delete;
	Mesh& operator = (Mesh&& other);

	// // ----- Addition Assign ----- // //
	Mesh& operator += (Mesh& other);

	// // ----- Append ----- // //
	void appendVertex(const Vertex s_shape);
	void appendTriangle(const Vertex v1, const Vertex v2, const Vertex v3);
	void appendIndex(const GLushort s_index);

	// // ----- Setters ----- // //
	void setVertex(std::size_t loc, Vertex& data);
	template <std::size_t attr>
	void setVertex(std::size_t loc, const glm::vec3& data)
	{
		assert(numVertices() >= loc);
		m_vertices.at(loc).position() = data;
	}
	void setIndex(std::size_t loc, const indexType& data);
	void setId(GLuint s_id);

	// // ----- Transform ----- // //
	void transform(glm::mat4 transformMatrix);
	void transform(verticesType& inVertices, const glm::mat4 transformMatrix);

	// // ----- IDs ----- // //
	void updateIds();

	// // ------INDICES ----- // //
	void makeIndicesSmooth();
	void makeIndicesFaceted();
	int findSimilarVertex(const indexType, const std::map<Vertex, indexType>&);

	// // ----- NORMALS ----- // //
	void makeNormals();

	// // ----- TANGENTS AND BITANGENTS ----- // //
	void makeTangents();

	// // ----- Getters ----- // //
	Vertex* getVertex(std::size_t i);
	template <std::size_t attr>
	glm::vec3 getVertex(const std::size_t i)
	{
		assert(m_num_vertices > 0);
		return std::get<attr>(m_vertices.at(i));
	}
	indexType getIndex(std::size_t i);

	std::size_t numIndices()	{ return m_indices.size(); }
	std::size_t numVertices()	{ return m_vertices.size(); }
	verticesType vertices()		{ return m_vertices; }
	const indicesType& indices() const { return m_indices; }

	// // ----- Size Getters ----- // //
	GLsizeiptr sizeVertices()	{ return m_vertices.size() * sizeof(Vertex); }
	GLsizeiptr sizeIndices()	{ return m_indices.size()  * sizeof(indexType); }
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
	GLuint			m_id;
};