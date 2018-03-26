#pragma once

#include "loadBMP_custom.h"
#include "targa.h"

struct Texture
{
	Texture(const char *filename);
	void upload_to_shader(unsigned int s_program_id, const char *gl_texture_name, const GLint s_texture_num);
private:
	void init_targa(const char *filename);
private:
	tga_image	m_image;
	GLuint		m_program_id;
	GLuint		m_tex_id;
	GLuint		m_tex_loc;
	GLuint		m_tex_num;
};