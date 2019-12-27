#ifndef GL_ENGINE_SHADING_LOAD_SHADER_H
#define GL_ENGINE_SHADING_LOAD_SHADER_H

#include <GL/glew.h>

namespace gl_engine
{
	/*
	
		Used for making a shader program from a vertex and fragment shader.
	
	*/
	namespace LoadShaders
	{
		GLuint load(const char * vertex_file_path, const char* fragment_file_path);
		GLuint load(const char * vertex_file_path, const char* geometry_file_path, const char* fragment_file_path);
	}
} // namespace gl_engine
#endif