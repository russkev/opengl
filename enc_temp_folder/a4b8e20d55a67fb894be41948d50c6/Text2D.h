#pragma once

#include <vector>
#include <tuple>
#include <iostream>

#include <glm/glm.hpp>

#include "Texture.h"
#include "Buffer.h"
#include "loadShader.hpp"
#include "VAO.h"

#include "GL_Type_Traits.h"
#include "GL_Tuple_Introspect.h"

struct Text2D
{
	// // TYPES // //
	typedef std::tuple< glm::vec2, glm::vec2>	vertex2DType;
	typedef std::vector<vertex2DType>			vertices2DType;

	// // ENUMERATION // //
	enum attr { position2D = 0, uv2D = 1 };

	// // CONSTRUCTOR // //
	Text2D(const char* s_texture_path);


	void print(const char* s_text, int s_x, int s_y, int size, Buffer& s_text_buffer, GLuint s_program_id);
	void cleanup();
private:
	Texture			m_texture;
	vertices2DType	m_vertices;
};