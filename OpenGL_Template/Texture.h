#pragma once

#include "loadBMP_custom.h"
#include "targa.h"

struct Texture
{
	Texture(unsigned int program_id, const char *filename, const char *gl_texturename, GLint s_minMagFilter, GLint s_tiling, GLint s_texture_num);
private:
	void init_targa(const char *filename);
private:
	tga_image	m_image;
	GLuint		m_program_id;
	GLuint		m_tex_id;
	GLuint		m_tex_loc;
	GLuint		m_tex_num;
};