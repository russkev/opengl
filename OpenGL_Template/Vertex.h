#pragma once
#include <tuple>
#include <glm/glm.hpp>
#include <GL/glew.h>

enum vertexAttr { position = 0, color = 1, normal = 2, uv = 3, id = 4, tangent = 5, bitangent = 6 };

struct Vertex
{
	// // ----- Consts ----- // //
	static constexpr float DISTANCE_THRESHOLD = 0.01f;

	// --- Don't forget to update the gl_introspect_tuple call when changing this --- //
	//                   Position,   Colour,    Normal,      UV,     ID,    Tangent,  Bitangent    //
	typedef std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint, glm::vec3, glm::vec3>	vertexType;

	// // ----- Big 6 ----- // //
	Vertex(glm::vec3, glm::vec3, glm::vec2);
	Vertex(glm::vec3, glm::vec3, glm::vec3, glm::vec2);
	Vertex(glm::vec3 s_position, glm::vec3 s_color, glm::vec3 s_normal, glm::vec2 s_uv, GLint id, glm::vec3 tangent, glm::vec3 bitangent);
	//Vertex();
	//~Vertex() {};
	Vertex(const Vertex&);
	Vertex(const Vertex&& other);
	//Vertex& operator = (const Vertex&) = delete;
	Vertex& operator = (Vertex&& other);

	// // ----- Getters ----- // //
	vertexType getVertexTuple();
	glm::vec3 getPosition();
	glm::vec3 getColor();
	glm::vec3 getNormal();
	glm::vec2 getUV();
	GLint getID();
	glm::vec3 getTangent();
	glm::vec3 getBitangent();

	// // ----- Setters ----- // //
	void setPosition(glm::vec3);
	void setColor(glm::vec3);
	void setNormal(glm::vec3);
	void setUV(glm::vec2);
	void setID(GLint);
	void setTangent(glm::vec3);
	void setBitangent(glm::vec3);

private:
	vertexType m_vertex;
};