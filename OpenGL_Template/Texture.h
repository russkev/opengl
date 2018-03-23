#pragma once

#include "loadBMP_custom.h"
#include "targa.h"

struct Texture
{
	Texture(unsigned int program_id, const char *filename, const char *gl_texturename, GLint s_minMagFilter, GLint s_tiling);
};