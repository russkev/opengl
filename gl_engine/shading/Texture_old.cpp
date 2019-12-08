#include "Texture_old.h"

Texture_old::Texture_old(const char *s_filename)	
{
	// Targa info: https://unix4lyfe.org/targa/
	init_targa(s_filename);
	m_id = Texture_old::m_next_id++;
}

// // CONSTRUCTOR // //
Texture_old::Texture_old():m_image(tga_image())
{}

// // CONSTRUCTOR // //
void Texture_old::init_targa(const char *filename)
{
	tga_result result;
	result = tga_read(&m_image, filename);
	if (result != TGA_NOERR)
	{
		fprintf(stderr, "ERROR: Opening '%s' failed: %s\n", filename, tga_error(result));
	}
	
	if (tga_is_top_to_bottom(&m_image))		tga_flip_vert(&m_image);
	if (tga_is_right_to_left(&m_image))		tga_flip_horiz(&m_image);
	if (tga_is_colormapped(&m_image))		tga_color_unmap(&m_image);
	if (tga_is_mono(&m_image))				tga_desaturate_avg(&m_image);

	tga_swap_red_blue(&m_image);
	tga_convert_depth(&m_image, 32);
}

// // DESTRUCTOR // //
Texture_old::~Texture_old()
{
	tga_free_buffers(&m_image);
}

// // MOVE CONSTRUCTOR // //
Texture_old::Texture_old(Texture_old&& other) :
	m_image(std::exchange(other.m_image, tga_image()))
{}

// // MOVE ASSIGN // //
Texture_old& Texture_old::operator = (Texture_old&& other)
{
	(*this).~Texture_old();
	return *new (this) Texture_old(std::move(other));
}



void Texture_old::upload_to_shader(unsigned int s_program_id, const char *gl_texture_name)
{
	glUseProgram(s_program_id);

	GLuint tex_id, tex_loc;
	glGenTextures(1, &tex_id);

	tex_loc = glGetUniformLocation(s_program_id, gl_texture_name);
	glUniform1i(tex_loc, m_id);

	glActiveTexture(GL_TEXTURE0 + m_id);
	// "Bind" the newly created texture : all future texture functions will modify this texture //
	glBindTexture(GL_TEXTURE_2D, tex_id);
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

int Texture_old::m_next_id = 0;