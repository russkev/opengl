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
	verticesType newVertices;
	indicesType newIndices;
	std::map<Vertex, indexType> vertexMap;

	for (int i = 0; i < m_vertices.size(); i++)
	{
		int existingIndex = findSimilarVertex(i, vertexMap);
		if (existingIndex >= 0)
		{
			newIndices.push_back((indexType)existingIndex);
		}
		else
		{

			newVertices.push_back(m_vertices.at(i));
			indexType newIndex = (indexType)newVertices.size() - 1;
			newIndices.push_back(newIndex);
			vertexMap[m_vertices.at(i)] = newIndex;
		}
	}
	std::swap(newVertices, m_vertices);
	std::swap(newIndices, m_indices);
}

int ShapeData::findSimilarVertex(const indexType s_currentVertIndex, const std::map<Vertex, indexType>& s_vertexMap)
{
	auto iterator = s_vertexMap.find(m_vertices.at(s_currentVertIndex));
	if (iterator == s_vertexMap.end())
	{
		return -1;
	}
	else
	{
		return iterator->second;
	}
}

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
	std::vector<indexType> existingIndices;

	// Cycle through sets of three indices
	for (auto i = 0; i < m_indices.size(); i += 3)
	{		
		// Get vertices
		const glm::vec3& v0 = m_vertices.at(m_indices.at(i + 0)).position();
		const glm::vec3& v1 = m_vertices.at(m_indices.at(i + 1)).position();
		const glm::vec3& v2 = m_vertices.at(m_indices.at(i + 2)).position();

		// Get UVs
		const glm::vec2& uv0 = m_vertices.at(m_indices.at(i + 0)).uv();
		const glm::vec2& uv1 = m_vertices.at(m_indices.at(i + 1)).uv();
		const glm::vec2& uv2 = m_vertices.at(m_indices.at(i + 2)).uv();

		// Edges of the triangle, position delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		// Calculate tangent and bitangent
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 newTangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 newBitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		// If tangent and bitangents exist, calculate the average of the current and the new tangents, otherwise just set the new tangent
		for (auto j = 0; j < 3; ++j){
			if (std::find(existingIndices.begin(), existingIndices.end(), m_indices.at(i + j)) != existingIndices.end()) {

				// Get existing tangents
				glm::vec3 existingTangent = m_vertices.at(m_indices.at(i + j)).tangent();
				glm::vec3 existingBitangent = m_vertices.at(m_indices.at(i + j)).bitangent();

				// Calculate and set the average of the tangents. Good for them to be non-normalised here.
				m_vertices.at(m_indices.at(i + j)).tangent() = (existingTangent + newTangent) / 2.0f;
				m_vertices.at(m_indices.at(i + j)).bitangent() = (existingBitangent + newBitangent) / 2.0f;

			}
			else {
				// Set the new tangents
				m_vertices.at(m_indices.at(i + j)).tangent() = newTangent;
				m_vertices.at(m_indices.at(i + j)).bitangent() = newBitangent;
			}
			
		}
		
		// Register used indices
		for (auto j = 0; j < 3; ++j) {
			existingIndices.push_back(m_indices.at(i + j));
		}
	}
}

void ShapeData::setId(GLuint s_id) 
{
	if (numVertices() > 0)
	{
		for (auto & vertex : m_vertices)
		{
			vertex.id() = s_id;
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
		auto position  = transformMatrix * glm::vec4(vertex.position(), 1);
		auto normal    = transformMatrix * glm::vec4(vertex.normal(),   1);
		auto tangent   = transformMatrix * glm::vec4(vertex.tangent(), 1);
		auto bitangent = transformMatrix * glm::vec4(vertex.bitangent(), 1);

		// Set the new vertex attributes
		vertex.position() = (glm::vec3)position;
		vertex.normal() = (glm::vec3)normal;
		vertex.tangent() = (glm::vec3)tangent;
		vertex.bitangent() = (glm::vec3)bitangent;
	}
}

void ShapeData::updateIds()
{
	if (numVertices() > 0)
	{
		for (auto & vertex : m_vertices)
		{
			vertex.id() = m_id;
		}
	}

}