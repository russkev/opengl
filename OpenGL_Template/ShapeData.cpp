#pragma once
#include "ShapeData.h"

#include <GL\glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <tuple>
#include <cassert>

// // ----- Constructors ----- // //
ShapeData::ShapeData() :
	m_vertices(verticesType{}),
	m_indices(indexType{}),
	m_num_vertices(0u),
	m_num_indices(0u),
	m_id(0u)
{};

ShapeData::ShapeData(const verticesType s_vertices, const indicesType s_indices) :
	m_vertices(s_vertices),
	m_indices(s_indices),
	m_num_vertices(m_num_vertices + s_vertices.size()),
	m_num_indices(m_num_indices + s_indices.size())
{};

// // ----- Move Constructor ----- // //
ShapeData::ShapeData(const ShapeData&& other) :
	m_vertices(std::move(other.m_vertices)),
	m_indices(std::move(other.m_indices)),
	m_num_vertices(std::move(other.m_num_vertices)),
	m_num_indices(std::move(other.m_num_indices)),
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
	for (std::size_t i = 0u; i < other.numIndices(); ++i)
	{
		m_indices.push_back(other.m_indices.at(i) + (GLushort)m_num_vertices);
	}
	m_vertices.insert(m_vertices.end(), other.m_vertices.begin(), other.m_vertices.end());
	m_num_indices += other.m_num_indices;
	m_num_vertices += other.m_num_vertices;
	updateIds();
	return *this;
}

// // ----- Append ----- // //
void ShapeData::append_vertices(const vertexDataType s_shape)
{
	m_vertices.push_back(vertexType{ std::get<position>(s_shape), std::get<color>(s_shape), std::get<normal>(s_shape), std::get<uv>(s_shape), m_id, glm::vec3(0,0,0), glm::vec3(0,0,0) });
	m_num_vertices++;
}
void ShapeData::append_indices(const GLushort s_index)
{
	m_indices.push_back(s_index);
	m_num_indices++;
}

// // ----- Setters ----- // //
void ShapeData::setVertex(std::size_t loc, const vertexType& data)
{
	assert(m_num_vertices >= loc);
	m_vertices.at(loc) = data;
}

// // ------INDICES ----- // //

// Used for the case when no indices are defined
void ShapeData::makeIndices()
{
	verticesType newVertices;
	for (int i = 0; i < m_vertices.size(); i++)
	{
		int existingIndex = findSimilarVertex(m_vertices.at(i));
		if (existingIndex >= 0)
		{
			m_indices.push_back((indexType)existingIndex);
		}
		else
		{
			newVertices.push_back(m_vertices.at(i));
			m_indices.push_back(i);
			m_num_indices++;
		}
	}
}

int ShapeData::findSimilarVertex(ShapeData::vertexType vertex)
{
	return -1;
}

void ShapeData::setIndex(std::size_t loc, const indexType& data)
{
	assert(m_num_indices >= loc);
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
		glm::vec3 & v0 = std::get<attr::position>(m_vertices.at(m_indices.at(i + 0)));
		glm::vec3 & v1 = std::get<attr::position>(m_vertices.at(m_indices.at(i + 1)));
		glm::vec3 & v2 = std::get<attr::position>(m_vertices.at(m_indices.at(i + 2)));

		// Get UVs
		glm::vec2 & uv0 = std::get<attr::uv>(m_vertices.at(m_indices.at(i + 0)));
		glm::vec2 & uv1 = std::get<attr::uv>(m_vertices.at(m_indices.at(i + 1)));
		glm::vec2 & uv2 = std::get<attr::uv>(m_vertices.at(m_indices.at(i + 2)));
		
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
				glm::vec3 existingtangent	= std::get<attr::tangent>(m_vertices.at(m_indices.at(i + j)));
				glm::vec3 existingBitangent = std::get<attr::bitangent>(m_vertices.at(m_indices.at(i + j)));

				std::get<attr::tangent>(m_vertices.at(m_indices.at(i + j))) = (existingtangent + newTangent) / 2.0f;
				std::get<attr::bitangent>(m_vertices.at(m_indices.at(i + j))) = (existingBitangent + newBitangent) / 2.0f;
			}
			else {
				std::get<attr::tangent>(m_vertices.at(m_indices.at(i + j))) = newTangent;
				std::get<attr::bitangent>(m_vertices.at(m_indices.at(i + j))) = newBitangent;
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
	if (m_num_vertices > 0)
	{
		for (auto & vertex : m_vertices)
		{
			std::get<attr::id>(vertex) = s_id;
		}
	}

}

// // ----- Getters ----- // //
ShapeData::vertexType ShapeData::getVertex(std::size_t i)
{
	assert(m_num_vertices >= i);
	return m_vertices.at(i);
}

ShapeData::indexType ShapeData::getIndex(std::size_t i)
{
	assert(m_num_indices >= i);
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
		auto position = transformMatrix * glm::vec4(std::get<attr::position>(vertex), 1);
		auto normal   = transformMatrix * glm::vec4(std::get<attr::normal>(vertex),   1);
		std::get<attr::position>(vertex) = (glm::vec3)position;
		std::get<attr::normal>(vertex) = (glm::vec3)normal;
		std::get<attr::tangent>(vertex) = (glm::vec3)tangent;
		std::get<attr::bitangent>(vertex) = (glm::vec3)bitangent;

	}
}

void ShapeData::updateIds()
{
	if (m_num_vertices > 0)
	{
		for (auto & vertex : m_vertices)
		{
			std::get<attr::id>(vertex) = m_id;
		}
	}

}