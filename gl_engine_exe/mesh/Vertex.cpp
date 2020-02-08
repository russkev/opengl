#pragma once
#include <tuple>
#include <cmath>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Vertex.h"
#include "../utils/VectorUtils.h"

namespace gl_engine_exe
{
	// // ----- CONSTRUCTORS ----- // //
	Vertex::Vertex() :
		Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f))
	{}

	Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv) :
		Vertex(position, glm::vec3(0.0f, 0.0f, 0.0f), normal, uv)
	{}

	Vertex::Vertex(glm::vec3 position, glm::vec3 color, glm::vec3 normal, glm::vec2 uv) :
		Vertex(position, color, normal, uv, 0, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f))
	{}

	Vertex::Vertex(glm::vec3 position, glm::vec3 color, glm::vec3 normal, glm::vec2 uv, GLint id, glm::vec3 tangent, glm::vec3 bitangent) :
		m_vertex({ position, color, normal, uv, id, tangent, bitangent })
	{}

	// // ----- OPERATOR OVERLOADS ----- // //
	bool Vertex::operator<(const Vertex& other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(Vertex)) > 0;
	}


	// // ----- GETTERS ----- // //
	const Vertex::vertexType& Vertex::vertex_tuple() const
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
	void Vertex::set_position(const glm::vec3& position)
	{
		std::get<vertexAttr::position_loc>(m_vertex) = position;
	}
	void Vertex::set_color(const glm::vec3& color)
	{
		std::get<vertexAttr::color_loc>(m_vertex) = color;
	}
	void Vertex::set_normal(const glm::vec3& normal)
	{
		std::get<vertexAttr::normal_loc>(m_vertex) = normal;
	}
	void Vertex::set_uv(const glm::vec2& uv)
	{
		std::get<vertexAttr::uv_loc>(m_vertex) = uv;
	}
	void Vertex::set_id(const GLint id)
	{
		std::get<vertexAttr::id_loc>(m_vertex) = id;
	}
	void Vertex::set_tangent(const glm::vec3& tangent)
	{
		std::get<vertexAttr::tangent_loc>(m_vertex) = tangent;
	}
	void Vertex::set_bitangent(const glm::vec3& bitangent)
	{
		std::get<vertexAttr::bitangent_loc>(m_vertex) = bitangent;
	}
} // namespace gl_engine