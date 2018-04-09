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
	typedef std::tuple< glm::vec2, glm::vec2, GLuint>	vertex2DType;
	typedef std::vector<vertex2DType>					vertices2DType;

	// // ENUMERATION // //
	enum attr { position2D = 0, uv2D = 1 };

	// // CONSTRUCTOR // //
	Text2D();

	void init(const char* s_texture_path, int s_x, int s_y, int s_size, int s_screen_width, int s_screen_height);
	void initVertices();
	void initShaders();
	void draw();
	int* string_to_array(const char* s_text);

	void print(const char* s_text);
	void cleanup();
public:
	Texture			m_texture;
	vertices2DType	m_vertices;
};