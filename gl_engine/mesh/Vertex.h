#pragma once
#include <tuple>
#include <glm/glm.hpp>
#include <GL/glew.h>

enum vertexAttr { position_loc = 0, color_loc = 1, normal_loc = 2, uv_loc = 3, id_loc = 4, tangent_loc = 5, bitangent_loc = 6 };

struct Vertex
{
	// // ----- CONSTS ----- // //
	static constexpr float DISTANCE_THRESHOLD = 0.01f;

	// // ----- TYPEDEF ----- // //
	// --- Don't forget to update the gl_introspect_tuple call when changing this --- //
	//                   Position,   Colour,    Normal,      UV,     ID,    Tangent,  Bitangent    //
	typedef std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint, glm::vec3, glm::vec3>	vertexType;

	// // ----- CONSTRUCTORS ----- // //
	Vertex();
	Vertex(glm::vec3, glm::vec3, glm::vec2);
	Vertex(glm::vec3, glm::vec3, glm::vec3, glm::vec2);
	Vertex(glm::vec3 s_position, glm::vec3 s_color, glm::vec3 s_normal, glm::vec2 s_uv, GLint id, glm::vec3 tangent, glm::vec3 bitangent);

	// // ----- OPERATOR OVERLOADS ----- // //
	bool operator < (const Vertex&) const;

	// // ----- GETTERS ----- // //
	const vertexType& vertexTuple() const;
	const glm::vec3& position() const;
	const glm::vec3& color() const;
	const glm::vec3& normal() const;
	const glm::vec2& uv() const;
	const GLint& id() const;
	const glm::vec3& tangent() const;
	const glm::vec3& bitangent() const;

	// // ----- SETTERS ----- // //
	glm::vec3& position();
	glm::vec3& color();
	glm::vec3& normal();
	glm::vec2& uv();
	GLint& id();
	glm::vec3& tangent();
	glm::vec3& bitangent();

private:
	// // ----- MEMBER VARIABLES ----- // //
	vertexType m_vertex;
};