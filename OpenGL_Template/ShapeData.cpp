#pragma once
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <tuple>
#include <cassert>
#include <string>
#include <map>
#include <string>

#include "ShapeData.h"
#include "VectorUtils.h"
#include "VertexIsSimilar.h"

// // ----- Constructors ----- // //
ShapeData::ShapeData() :
	m_vertices(verticesType{}),
	m_indices(indexType{}),
	m_id(0u)
{};

ShapeData::ShapeData(const verticesType s_vertices, const indicesType s_indices) :
	m_vertices(s_vertices),
	m_indices(s_indices)
{};

// // ----- Move Constructor ----- // //
ShapeData::ShapeData(const ShapeData&& other) :
	m_vertices(std::move(other.m_vertices)),
	m_indices(std::move(other.m_indices)),
	m_id(std::move(other.m_id))
{};

// // ----- Move Assign ----- // //
ShapeData& ShapeData::operator = (ShapeData&& other)
{
	(*this).~ShapeData();
	return *new (this) ShapeData(std::move(other));
}

// // ----- Addition Assign ----- // //
ShapeData& ShapeData::operator += (ShapeData& other)
{
	auto currentNumVertices = (indexType)numVertices();
	for (std::size_t i = 0u; i < other.numIndices(); ++i)
	{
		m_indices.push_back(other.m_indices.at(i) + currentNumVertices);
	}
	m_vertices.insert(m_vertices.end(), other.m_vertices.begin(), other.m_vertices.end());
	updateIds();
	return *this;
}

// // ----- Append ----- // //
void ShapeData::append_vertices(const Vertex s_vertex)
{
	m_vertices.push_back(s_vertex);
}
void ShapeData::append_indices(const GLushort s_index)
{
	m_indices.push_back(s_index);
}

// // ----- Setters ----- // //
void ShapeData::setVertex(std::size_t loc, Vertex& vertex)
{
	assert(numVertices() >= loc);
	m_vertices.at(loc) = std::move(vertex);
}

// // ------INDICES ----- // //

// Guess shared indices based on proximity
void ShapeData::makeIndices()
{
	for (auto i = 0; i < m_vertices.size(); i++)
	{
		m_indices.push_back((indexType)i);
	}
	//verticesType newVertices;
	//indicesType newIndices;
	//bool(*fn_pt)(vertexType, vertexType) = tempCompare;
	////std::map<vertexType, indexType, std::function<bool(const vertexType &, const vertexType &)>> vertexMap(tempCompare);

	//std::map<vertexType, indexType> vertexMap;

	//for (int i = 0; i < m_vertices.size(); i++)
	//{
	//	int existingIndex = -1;
	//	//if (newVertices.size() != 0)
	//	//{
	//	//	auto iterator = vertexMap.find(m_vertices.at(i));
	//	//	if (iterator != vertexMap.end())
	//	//	{
	//	//		existingIndex = iterator->second;
	//	//	}
	//	//}
	//	if (existingIndex >= 0)
	//	{
	//		newIndices.push_back((indexType)existingIndex);
	//	}
	//	else
	//	{

	//		newVertices.push_back(m_vertices.at(i));
	//		indexType newIndex = (indexType)newVertices.size() - 1;
	//		newIndices.push_back(newIndex);
	//		vertexMap[m_vertices.at(i)] = newIndex;
	//	}
	//}
	//std::swap(newVertices, m_vertices);
	//std::swap(newIndices, m_indices);
}

//int ShapeData::findSimilarVertex(const indexType s_currentVertIndex, std::map<vertexType, indexType, VertexCompare> s_vertexMap)
//{
//	auto iterator = s_vertexMap.find(m_vertices.at(s_currentVertIndex));
//	if (iterator == s_vertexMap.end())
//	{
//		return -1;
//	}
//	else
//	{
//		return iterator->second;
//	}
//	return -1;
//}

void ShapeData::setIndex(std::size_t loc, const indexType& data)
{
	assert(numIndices() >= loc);
	m_indices.at(loc) = data;
}

// // ----- TANGENTS AND BITANGENTS ----- // //
void ShapeData::makeTangents()
{
	assert(m_vertices.size() > 0);
	assert(m_indices.size() > 0);
	std::vector<GLushort> existingIndices;
	for (auto i = 0; i < m_indices.size(); i += 3)
	{		
		// Get vertices
		//glm::vec3 & v0 = std::get<attr::position>(m_vertices.at(m_indices.at(i + 0)));
		//glm::vec3 & v1 = std::get<attr::position>(m_vertices.at(m_indices.at(i + 1)));
		//glm::vec3 & v2 = std::get<attr::position>(m_vertices.at(m_indices.at(i + 2)));
		glm::vec3 & v0 = m_vertices.at(m_indices.at(i + 0)).getPosition();
		glm::vec3 & v1 = m_vertices.at(m_indices.at(i + 1)).getPosition();
		glm::vec3 & v2 = m_vertices.at(m_indices.at(i + 2)).getPosition();


		// Get UVs
		//glm::vec2 & uv0 = std::get<attr::uv>(m_vertices.at(m_indices.at(i + 0)));
		//glm::vec2 & uv1 = std::get<attr::uv>(m_vertices.at(m_indices.at(i + 1)));
		//glm::vec2 & uv2 = std::get<attr::uv>(m_vertices.at(m_indices.at(i + 2)));
		glm::vec2 & uv0 = m_vertices.at(m_indices.at(i + 0)).getUV();
		glm::vec2 & uv1 = m_vertices.at(m_indices.at(i + 0)).getUV();
		glm::vec2 & uv2 = m_vertices.at(m_indices.at(i + 0)).getUV();

		
		// Edges of the triangle, position delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 newTangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 newBitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		for (auto j = 0; j < 3; ++j){
			if (std::find(existingIndices.begin(), existingIndices.end(), m_indices.at(i + j)) != existingIndices.end()) {
				int temp = m_indices.at(i + j);
				//glm::vec3 existingtangent	= std::get<attr::tangent>(m_vertices.at(m_indices.at(i + j)));
				//glm::vec3 existingBitangent = std::get<attr::bitangent>(m_vertices.at(m_indices.at(i + j)));

				glm::vec3 existingTangent = m_vertices.at(m_indices.at(i + j)).getTangent();
				glm::vec3 existingBitangent = m_vertices.at(m_indices.at(i + j)).getBitangent();

				//std::get<attr::tangent>(m_vertices.at(m_indices.at(i + j))) = (existingtangent + newTangent) / 2.0f;
				//std::get<attr::bitangent>(m_vertices.at(m_indices.at(i + j))) = (existingBitangent + newBitangent) / 2.0f;
				m_vertices.at(m_indices.at(i + j)).setTangent((existingTangent + newTangent) / 2.0f);
				m_vertices.at(m_indices.at(i + j)).setBitangent((existingBitangent + newBitangent) / 2.0f);

			}
			else {
				//std::get<attr::tangent>(m_vertices.at(m_indices.at(i + j))) = newTangent;
				//std::get<attr::bitangent>(m_vertices.at(m_indices.at(i + j))) = newBitangent;
				m_vertices.at(m_indices.at(i + j)).setTangent(newTangent);
				m_vertices.at(m_indices.at(i + j)).setBitangent(newBitangent);
			}
			
		}
		
		// Register used indices
		for (auto j = 0; j < 3; ++j) {
			existingIndices.push_back(m_indices.at(i + j));
		}
		auto k = 0;
	}
}

void ShapeData::setId(GLuint s_id) 
{
	if (numVertices() > 0)
	{
		for (auto & vertex : m_vertices)
		{
			vertex.setID(s_id);
		}
	}

}

// // ----- Getters ----- // //
Vertex* ShapeData::getVertex(std::size_t loc)
{
	assert(numVertices() >= loc);
	return &m_vertices.at(loc);
}

ShapeData::indexType ShapeData::getIndex(std::size_t i)
{
	assert(numIndices() >= i);
	return m_indices.at(i);
}

 // ----- Transform ----- // //
void ShapeData::transform(glm::mat4 transformMatrix) 
{
	transform(m_vertices, transformMatrix);
}

void ShapeData::transform(ShapeData::verticesType& inVertices, const glm::mat4 transformMatrix)
{

	assert(inVertices.size() > 0);
	for (auto & vertex : inVertices)
	{
		// Transform the vertex
		auto position  = transformMatrix * glm::vec4(vertex.getPosition(), 1);
		auto normal    = transformMatrix * glm::vec4(vertex.getNormal(),   1);
		auto tangent   = transformMatrix * glm::vec4(vertex.getTangent(), 1);
		auto bitangent = transformMatrix * glm::vec4(vertex.getBitangent(), 1);

		// Set the new vertex attributes
		vertex.setPosition((glm::vec3)position);
		vertex.setNormal((glm::vec3)normal);
		vertex.setNormal((glm::vec3)tangent);
		vertex.setNormal((glm::vec3)bitangent);

	}
}

void ShapeData::updateIds()
{
	if (numVertices() > 0)
	{
		for (auto & vertex : m_vertices)
		{
			vertex.setID(m_id);
		}
	}

}