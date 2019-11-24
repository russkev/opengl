#pragma once
#include <tuple>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Vertex.h"

// // ----- Big 6 ----- // //
Vertex::Vertex(glm::vec3 s_position, glm::vec3 s_normal, glm::vec2 s_uv) :
	Vertex(s_position, glm::vec3(0.0f, 0.0f, 0.0f), s_normal, s_uv)
{}

Vertex::Vertex(glm::vec3 s_position, glm::vec3 s_color, glm::vec3 s_normal, glm::vec2 s_uv) :
	Vertex(s_position, s_color, s_normal, s_uv, 0, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f))
{}

Vertex::Vertex(glm::vec3 s_position, glm::vec3 s_colour, glm::vec3 s_normal, glm::vec2 s_uv, GLint s_id, glm::vec3 s_tangent, glm::vec3 s_bitangent) :
	m_vertex({ s_position, s_colour, s_normal, s_uv, s_id, s_tangent, s_bitangent })
{}

Vertex::Vertex(const Vertex& other) : m_vertex(other.m_vertex)
{}

Vertex::Vertex(const Vertex&& other) : 
	m_vertex(std::move(other.m_vertex))
{}

Vertex& Vertex::operator = (Vertex&& other)
{
	(*this).~Vertex();
	return *new (this) Vertex(std::move(other));
}

// // ----- Getters ----- // //
Vertex::vertexType Vertex::getVertexTuple()
{
	return m_vertex;
}

glm::vec3 Vertex::getPosition()
{
	return std::get<position>(m_vertex);
}

glm::vec3 Vertex::getColor()
{
	return std::get<color>(m_vertex);
}

glm::vec3 Vertex::getNormal()
{
	return std::get<normal>(m_vertex);
}

glm::vec2 Vertex::getUV()
{
	return std::get<uv>(m_vertex);
}

GLint Vertex::getID()
{
	return std::get<id>(m_vertex);
}

glm::vec3 Vertex::getTangent()
{
	return std::get<tangent>(m_vertex);
}

glm::vec3 Vertex::getBitangent()
{
	return std::get<bitangent>(m_vertex);
}

// // ----- Setters ----- // //
void Vertex::setPosition(glm::vec3 s_position)
{
	std::get<position>(m_vertex) = s_position;
}

void Vertex::setColor(glm::vec3 s_color)
{
	std::get<color>(m_vertex) = s_color;
}

void Vertex::setNormal(glm::vec3 s_normal)
{
	std::get<normal>(m_vertex) = s_normal;
}

void Vertex::setUV(glm::vec2 s_uv)
{
	std::get<uv>(m_vertex) = s_uv;
}

void Vertex::setID(GLint s_id)
{
	std::get<id>(m_vertex) = s_id;
}

void Vertex::setTangent(glm::vec3 s_tangent)
{
	std::get<tangent>(m_vertex) = s_tangent;
}

void Vertex::setBitangent(glm::vec3 s_bitangent)
{
	std::get<bitangent>(m_vertex) = s_bitangent;
}