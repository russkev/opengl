#include "Texture.h"

Texture::Texture(const char *s_filename)	
{
	// Targa info: https://unix4lyfe.org/targa/
	init_targa(s_filename);

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

void Texture::upload_to_shader(unsigned int s_program_id, const char *gl_texture_name, const GLint s_texture_num)
{
	m_program_id = s_program_id;
	m_tex_num = s_texture_num;
	glUseProgram(m_program_id);

	//GLuint tex_id;
	glGenTextures(1, &m_tex_id);

	m_tex_loc = glGetUniformLocation(m_program_id, gl_texture_name);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
}