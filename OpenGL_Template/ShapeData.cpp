#pragma once
#include "ShapeData.h"
#include "Vertex.h"

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
	m_id(5u)
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
	m_vertices.push_back({ std::get<attr::position>(s_shape),std::get<attr::color>(s_shape), std::get<attr::normal>(s_shape), m_id });
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

void ShapeData::setIndex(std::size_t loc, const indexType& data)
{
	assert(m_num_indices >= loc);
	m_indices.at(loc) = data;
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