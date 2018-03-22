#include "Texture.h"

Texture::Texture(const tga_image& s_image, GLint s_minMagFilter, GLint s_tiling)
{
	GLuint textureID;
	//glGenTextures(1, &textureID);
	//// "Bind" the newly created texture : all future texture functions will modify this texture //
	//glBindTexture(GL_TEXTURE_2D, textureID);
	//// Give the image to OpenGL //
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, s_image.width, s_image.height, 0, GL_BGR, GL_UNSIGNED_BYTE, s_image.image_data);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, s_minMagFilter);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, s_minMagFilter);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_tiling);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, s_tiling);
}