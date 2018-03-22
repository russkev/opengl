#pragma once

#include "loadBMP_custom.h"
#include "targa.h"

struct Texture
{
	Texture(const tga_image& s_image, GLint s_minMagFilter, GLint s_tiling);
};