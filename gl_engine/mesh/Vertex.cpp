#pragma once
#include <tuple>
#include <cmath>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Vertex.h"
#include "../utils/VectorUtils.h"

// // ----- CONSTRUCTORS ----- // //
Vertex::Vertex() :
	Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f))
{}

Vertex::Vertex(glm::vec3 s_position, glm::vec3 s_normal, glm::vec2 s_uv) :
	Vertex(s_position, glm::vec3(0.0f, 0.0f, 0.0f), s_normal, s_uv)
{}

Vertex::Vertex(glm::vec3 s_position, glm::vec3 s_color, glm::vec3 s_normal, glm::vec2 s_uv) :
	Vertex(s_position, s_color, s_normal, s_uv, 0, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f))
{}

Vertex::Vertex(glm::vec3 s_position, glm::vec3 s_colour, glm::vec3 s_normal, glm::vec2 s_uv, GLint s_id, glm::vec3 s_tangent, glm::vec3 s_bitangent) :
	m_vertex({ s_position, s_colour, s_normal, s_uv, s_id, s_tangent, s_bitangent })
{}

// // ----- OPERATOR OVERLOADS ----- // //
bool Vertex::operator<(const Vertex& other) const
{
	return memcmp((void*)this, (void*)&other, sizeof(Vertex)) > 0;
}


// // ----- GETTERS ----- // //
const Vertex::vertexType& Vertex::vertexTuple() const
{
	return m_vertex;
}

const glm::vec3& Vertex::position() const
{
	return std::get<vertexAttr::position_loc>(m_vertex);
}

const glm::vec3& Vertex::color() const
{
	return std::get<color_loc>(m_vertex);
}

const glm::vec3& Vertex::normal() const
{
	return std::get<normal_loc>(m_vertex);
}

const glm::vec2& Vertex::uv() const
{
	return std::get<uv_loc>(m_vertex);
}

const GLint& Vertex::id() const
{
	return std::get<id_loc>(m_vertex);
}

const glm::vec3& Vertex::tangent() const
{
	return std::get<tangent_loc>(m_vertex);
}

const glm::vec3& Vertex::bitangent() const
{
	return std::get<bitangent_loc>(m_vertex);
}

// // ----- SETTERS ----- // //
glm::vec3& Vertex::position ()
{
	return std::get<vertexAttr::position_loc>(m_vertex);
}

glm::vec3& Vertex::color()
{
	return std::get<vertexAttr::color_loc>(m_vertex);
}

glm::vec3& Vertex::normal()
{
	return std::get<vertexAttr::normal_loc>(m_vertex);
}

glm::vec2& Vertex::uv()
{
	return std::get<vertexAttr::uv_loc>(m_vertex);
}

GLint& Vertex::id()
{
	return std::get<vertexAttr::id_loc>(m_vertex);
}

glm::vec3& Vertex::tangent()
{
	return std::get<vertexAttr::tangent_loc>(m_vertex);
}

glm::vec3& Vertex::bitangent()
{
	return std::get<vertexAttr::bitangent_loc>(m_vertex);
}