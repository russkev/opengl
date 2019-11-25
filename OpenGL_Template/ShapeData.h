#pragma once
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <tuple>
#include <cassert>
#include <string>
#include <map>
#include <cstdlib>

#include "VectorUtils.h"
#include "Vertex.h"

struct ShapeData 
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
	void append_vertices(const Vertex s_shape);
	void append_indices(const GLushort s_index);

	// // ----- Setters ----- // //
	void setVertex(std::size_t loc, Vertex& data);
	template <std::size_t attr>
	void setVertex(std::size_t loc, const glm::vec3& data)
	{
		assert(numVertices() >= loc);
		m_vertices.at(loc).setPosition(data);
		//std::get<attr>(m_vertices.at(loc)) = data;
	}
	void setIndex(std::size_t loc, const indexType& data);
	void setId(GLuint s_id);

	// // ----- Transform ----- // //
	void transform(glm::mat4 transformMatrix);
	void transform(verticesType& inVertices, const glm::mat4 transformMatrix);

	// // ----- IDs ----- // //
	void updateIds();

	// // ------INDICES ----- // //
	void makeIndices();
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
	indicesType indices()		{ return m_indices; }

	// // ----- Size Getters ----- // //
	GLsizeiptr sizeVertices()	{ return m_vertices.size() * sizeof(Vertex); }
	GLsizeiptr sizeIndices()	{ return m_indices.size()  * sizeof(indexType); }
	GLsizeiptr sizeShape()		{ return sizeVertices()    + sizeIndices(); }

	// // ----- Iterators ----- // //
	vrt_iterator vert_begin()	{ return m_vertices.begin(); }
	vrt_iterator vert_end()		{ return m_vertices.end(); }
	ind_iterator indx_begin()	{ return m_indices.begin(); }
	ind_iterator indx_end()		{ return m_indices.end(); }
	
	// // ----- Operator Overload ----- // //
private:
	// // ----- Member Variables ----- // //
	verticesType	m_vertices;
	indicesType		m_indices;
	//std::size_t		m_num_vertices;
	//std::size_t		m_num_indices;
	GLuint			m_id;
};


//struct VertexCompare
//{
//	//static constexpr float DISTANCE_THRESHOLD = 0.01f;
//	bool operator () (const ShapeData::vertexType& v1, const ShapeData::vertexType& v2) const 
//	{
//		glm::vec3 v1_p = std::get<ShapeData::attr::position>(v1);
//		glm::vec3 v2_p = std::get<ShapeData::attr::position>(v2);
//		glm::vec2 v1_uv = std::get<ShapeData::attr::uv>(v1);
//		glm::vec2 v2_uv = std::get<ShapeData::attr::uv>(v2);
//		glm::vec3 v1_n = std::get<ShapeData::attr::normal>(v1);
//		glm::vec3 v2_n = std::get<ShapeData::attr::normal>(v2);
//
//		//if (
//		//	VectorUtils::isNear(v1_p, v2_p, ShapeData::DISTANCE_THRESHOLD) &&
//		//	VectorUtils::isNear(v1_uv, v2_uv, ShapeData::DISTANCE_THRESHOLD) &&
//		//	VectorUtils::isNear(v1_n, v2_n, ShapeData::DISTANCE_THRESHOLD))
//		//{
//		//	return false;
//		//}
//		/*return v1_p.x <= v2_p.x;*/
//		return
//			//v1_p.x < v2_p.x - ShapeData::DISTANCE_THRESHOLD / 2 &&
//			//v1_p.y < v2_p.y - ShapeData::DISTANCE_THRESHOLD / 2 &&
//			//v1_p.z < v2_p.z - ShapeData::DISTANCE_THRESHOLD / 2 /*&&
//			//v1_uv.s < v2_uv.s - ShapeData::DISTANCE_THRESHOLD / 2 &&
//			//v1_uv.t < v2_uv.t - ShapeData::DISTANCE_THRESHOLD / 2 &&
//			//v1_n.x < v2_n.x - ShapeData::DISTANCE_THRESHOLD / 2 &&
//			//v1_n.y < v2_n.y - ShapeData::DISTANCE_THRESHOLD / 2 &&
//			//v1_n.z < v2_n.z - ShapeData::DISTANCE_THRESHOLD / 2*/;
//			v1_p.x < v2_p.x &&
//			v1_p.y < v2_p.y &&
//			v1_p.z < v2_p.z;
//	}
//};