#include "Texture.h"

Texture::Texture(unsigned int s_program_id, const char *s_filename, const char *gl_texturename, GLint s_minMagFilter, GLint s_tiling, GLint s_texture_num):
	m_program_id(s_program_id), m_tex_num(s_texture_num)
{
	// Targa info: https://unix4lyfe.org/targa/
	init_targa(s_filename);
	glUseProgram(m_program_id);

	//GLuint tex_id;
	glGenTextures(1, &m_tex_id);

	m_tex_loc = glGetUniformLocation(m_program_id, gl_texturename);
	glUniform1i(m_tex_loc, m_tex_num);

	glActiveTexture(GL_TEXTURE0 + m_tex_num);
	// "Bind" the newly created texture : all future texture functions will modify this texture //
	glBindTexture(GL_TEXTURE_2D, m_tex_id);
	// Give the image to OpenGL //
	glTexImage2D
	(
		GL_TEXTURE_2D,		// Target
		0,					// Level
		GL_RGBA,			// Internal Format
		m_image.width,		// Width
		m_image.height,		// Height
		0,					// Border
		GL_RGBA,			// Format
		GL_UNSIGNED_BYTE,	// Type
		m_image.image_data	// * Data
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, s_minMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, s_minMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_tiling);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, s_tiling);
}

void Texture::init_targa(const char *filename)
{
	tga_image img;
	tga_result result;
	result = tga_read(&m_image, filename);
	if (result != TGA_NOERR)
	{
		fprintf(stderr, "ERROR: Opening '%s' failed: %s\n", filename, tga_error(result));
	}

	
	if (!tga_is_top_to_bottom(&m_image))	tga_flip_vert(&m_image);
	if (tga_is_right_to_left(&m_image))		tga_flip_horiz(&m_image);
	if (tga_is_colormapped(&m_image))		tga_color_unmap(&m_image);
	if (tga_is_mono(&m_image))				tga_desaturate_avg(&m_image);

	tga_swap_red_blue(&m_image);
	tga_convert_depth(&m_image, 32);
}