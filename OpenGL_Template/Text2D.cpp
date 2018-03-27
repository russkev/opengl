#include "Text2D.h"

// // CONSTRUCTOR
Text2D::Text2D(const char* s_texture_path) 
{
	m_texture = Texture(s_texture_path);
}

void Text2D::print(const char* s_text, int x, int y, int size)
{
	//for (unsigned int i = 0;)
}
void Text2D::cleanup()
{
	//
}