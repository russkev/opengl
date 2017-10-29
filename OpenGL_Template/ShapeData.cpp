#pragma once
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <tuple>
#include <cassert>

#include "Vertex.h"
#include "ShapeData.h"

// // ----- Type Definitions ----- // //
typedef std::tuple<glm::vec3, glm::vec3, glm::vec3> shapeType;
typedef std::vector<shapeType>						vertexType;
typedef std::vector<GLushort>						indexType;

// // ----- Constructors ----- // //
ShapeData::ShapeData() :
	m_vertices(vertexType{}),
	m_indices(indexType{}),
	m_num_vertices(0u),
	m_num_indices(0u)
{};
ShapeData::ShapeData(const vertexType s_vertices, const indexType s_indices) :
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
	m_num_indices(std::move(other.m_num_indices))
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
	return *this;
}

// // ----- Append ----- // //
void ShapeData::append_vertices(const shapeType s_shape)
{
	m_vertices.push_back(s_shape);
	m_num_vertices++;
}
void ShapeData::append_indices(const GLushort s_index)
{
	m_indices.push_back(s_index);
	m_num_indices++;
}

// // ----- Setters ----- // //
//void ShapeData::setVertex(std::size_t loc, const shapeType& data)
//{
//	assert(m_num_vertices >= loc);
//	m_vertices.at(loc) = data;
//}
//template <std::size_t attr>
//void ShapeData::setVertex(std::size_t loc, const glm::vec3& data)
//{
//	assert(m_num_vertices >= loc);
//	std::get<attr>(m_vertices.at(loc)) = data;
//}

// // ----- Getters ----- // //
shapeType ShapeData::getVertex(std::size_t i)
{
	assert(m_num_vertices > i);
	return m_vertices.at(i);
}
template <std::size_t attr>
glm::vec3 ShapeData::getVertex(const std::size_t i)
{
	assert(m_num_vertices > 0);
	return std::get<attr>(m_vertices.at(i));
}