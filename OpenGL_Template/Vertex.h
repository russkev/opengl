#pragma once
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex {
	glm::tvec3<GLfloat> position;
	glm::tvec3<GLfloat> color;

	
	Vertex(glm::tvec3<GLfloat> s_position, glm::tvec3<GLfloat> s_color) :
		position(s_position), color(s_color) {};
};