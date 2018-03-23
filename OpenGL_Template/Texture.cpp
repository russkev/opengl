#include "Texture.h"

Texture::Texture(unsigned int program_id, const char *filename, const char *gl_texturename, GLint s_minMagFilter, GLint s_tiling, GLint s_num)
{
	// Targa info: https://unix4lyfe.org/targa/
	tga_image image;
	tga_read(&image, filename);
	if (!tga_is_top_to_bottom(&image))	tga_flip_vert(&image);
	if (tga_is_right_to_left(&image))	tga_flip_horiz(&image);
	if (tga_is_colormapped(&image))		tga_color_unmap(&image);
	if (tga_is_mono(&image))			tga_desaturate_avg(&image);

	tga_swap_red_blue(&image);
	tga_convert_depth(&image, 32);
	auto colorFormat = GL_RGBA;

	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0 + s_num);
	// "Bind" the newly created texture : all future texture functions will modify this texture //
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Give the image to OpenGL //
	glTexImage2D
	(
		GL_TEXTURE_2D,		// Target
		0,					// Level
		colorFormat,		// Internal Format
		image.width,		// Width
		image.height,		// Height
		0,					// Border
		colorFormat,		// Format
		GL_UNSIGNED_BYTE,	// Type
		image.image_data	// * Data
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, s_minMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, s_minMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_tiling);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, s_tiling);

	GLint texture_loc = glGetUniformLocation(program_id, gl_texturename);

	glUniform1i(texture_loc, 0);
}