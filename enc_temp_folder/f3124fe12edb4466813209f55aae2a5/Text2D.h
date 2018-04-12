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

static constexpr auto MAX_LETTERS = 200u;

struct Text2D
{
	// // TYPES // //
	typedef std::tuple< glm::vec2, glm::vec2, GLuint>	vertex2DType;
	typedef std::vector<vertex2DType>					vertices2DType;

	// // ENUMERATION // //
	enum attr { position2D = 0, uv2D = 1 };

	// // CONSTRUCTOR // //
	Text2D(const char* s_texture_path);

	void init(int s_x, int s_y, int size, int s_screen_width, int s_screen_height);
	void initVertices();
	void initShaders();

	void print(const char* s_text);
	void printToConsole(const int* charArray);
	void draw();
	int* string_to_array(const char* s_text);

	void cleanup();
public:
	Texture			m_texture;
	vertices2DType	m_vertices;
	int				m_x, m_y, m_size;
	GLuint			m_program_id, m_width_uniform_id, m_height_uniform_id, m_string_uniform_id, m_screen_width, m_screen_height;
	Buffer			m_buffer = { GL_ARRAY_BUFFER, 0 };
	VAO				m_vao;
};