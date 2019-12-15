#ifndef GL_ENGINE_SHADING_LOAD_SHADER_H
#define GL_ENGINE_SHADING_LOAD_SHADER_H

#include <GL/glew.h>

namespace gl_engine
{
	namespace LoadShaders
	{
		GLuint load(const char * vertex_file_path, const char* fragment_file_path);
	}
} // namespace gl_engine
#endif