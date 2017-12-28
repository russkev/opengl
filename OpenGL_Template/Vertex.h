#pragma once
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex {
	glm::tvec3<GLfloat> position;
	glm::tvec3<GLfloat> color;
	glm::tvec3<GLfloat> normal;
	GLushort id;

	Vertex(const glm::tvec3<GLfloat> s_position, glm::tvec3<GLfloat> s_color, glm::tvec3<GLfloat> s_normal) :
		position(s_position), color(s_color), normal(s_normal), id(0u) {};
	
	Vertex(const glm::tvec3<GLfloat> s_position, glm::tvec3<GLfloat> s_color) :
		position(s_position), color(s_color), normal(glm::tvec3<GLfloat>()), id(0u) {};

	Vertex(const glm::tvec3<GLfloat> s_position) :
		position(s_position), color(glm::tvec3<GLfloat>()), normal(glm::tvec3<GLfloat>()), id(0u) {};

};