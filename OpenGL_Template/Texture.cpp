#include "Texture.h"

Texture::Texture(unsigned int program_id, const char *filename, const char *gl_texturename, GLint s_minMagFilter, GLint s_tiling)
{
	// Targa info: https://unix4lyfe.org/targa/
	tga_image image;
	tga_read(&image, filename);

	GLuint textureID;
	glGenTextures(1, &textureID);
	// "Bind" the newly created texture : all future texture functions will modify this texture //
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Give the image to OpenGL //
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_BGR, GL_UNSIGNED_BYTE, image.image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, s_minMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, s_minMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_tiling);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, s_tiling);

	GLint texture_loc = glGetUniformLocation(program_id, gl_texturename);
	glUniform1i(texture_loc, 0);
}