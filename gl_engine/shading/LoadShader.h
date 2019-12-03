#include <GL/glew.h>
#pragma once

namespace LoadShaders
{
	GLuint load(const char * vertex_file_path, const char* fragment_file_path);
}
