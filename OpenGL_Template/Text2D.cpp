#include "Text2D.h"

// // CONSTRUCTOR
Text2D::Text2D(const char* s_texture_path):
	m_texture(Texture(s_texture_path))
{}

void Text2D::print(const char* s_text, int s_x, int s_y, int s_size)
{
	unsigned int offset = 32;
	unsigned int length = strlen(s_text);
	for (unsigned int i = 0; i < length; ++i)
	{
		char character = s_text[i] - offset;
		float uv_size = 1.0f / 16.0f;
		float uv_x = (character % 16) * uv_size;
		float uv_y = (character / 16) * uv_size;
		
		vertex2DType top_left		= { { s_x + i * s_size,				s_y + s_size	}, { uv_x,				1.0 - uv_y				} };
		vertex2DType top_right		= { { s_x + i * s_size + s_size,	s_y + s_size	}, { uv_x + uv_size,	1.0 - uv_y				} };
		vertex2DType bottom_left	= { { s_x + i * s_size,				s_y				}, { uv_x,				1.0 - uv_y -  uv_size	} };
		vertex2DType bottom_right	= { { s_x + i * s_size + s_size,	s_y				}, { uv_x + uv_size,	1.0 - uv_y -  uv_size	} };

		m_vertices.push_back(top_left);
		m_vertices.push_back(bottom_left);
		m_vertices.push_back(top_right);

		m_vertices.push_back(top_right);
		m_vertices.push_back(bottom_left);
		m_vertices.push_back(bottom_right);
	}
	
}
void Text2D::cleanup()
{
	//
}