#pragma once

#include <vector>
#include <tuple>

#include <glm/glm.hpp>

#include "Texture.h"
#include "Buffer.h"

struct Text2D
{
	// // TYPES // //
	typedef std::tuple< glm::vec2, glm::vec2>	vertex2DType;
	typedef std::vector<vertex2DType>			vertices2DType;

	// // ENUMERATION // //
	enum attr { position2D = 0, uv2D = 1 };

	// // CONSTRUCTOR // //
	Text2D(const char* s_texture_path);


	void print(const char* s_text, int s_x, int s_y, int size);
	void cleanup();
private:
	Texture			m_texture;
	vertices2DType	m_vertices;
};