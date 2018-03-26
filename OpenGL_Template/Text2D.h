#pragma once

#include <vector>
#include <tuple>

#include <glm/glm.hpp>

#include "Texture.h"

struct Text2D
{
	typedef std::tuple< glm::vec2, glm::vec2> vertex2DType;
	Text2D(const char* s_texture_path);
	void print(const char* s_text, int x, int y, int size);
	void cleanup();
private:
	Texture			m_texture;
	vertex2DType	m_vertices;
};