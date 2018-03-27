#include "Texture.h"

Texture::Texture(const char *s_filename)	
{
	// Targa info: https://unix4lyfe.org/targa/
	init_targa(s_filename);
}

// // CONSTRUCTOR // //
Texture::Texture():m_image(tga_image())
{}

// // CONSTRUCTOR // //
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

// // DESTRUCTOR // //
Texture::~Texture()
{
	tga_free_buffers(&m_image);
}

// // MOVE CONSTRUCTOR // //
Texture::Texture(Texture&& other) :
	m_image(std::exchange(other.m_image, tga_image()))
{}

// // MOVE ASSIGN // //
Texture& Texture::operator = (Texture&& other)
{
	(*this).~Texture();
	return *new (this) Texture(std::move(other));
}



void Texture::upload_to_shader(unsigned int s_program_id, const char *gl_texture_name, const GLint s_texture_num)
{
	glUseProgram(s_program_id);

	GLuint tex_id, tex_loc;
	glGenTextures(1, &tex_id);

	tex_loc = glGetUniformLocation(s_program_id, gl_texture_name);
	glUniform1i(tex_loc, s_texture_num);

	glActiveTexture(GL_TEXTURE0 + s_texture_num);
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