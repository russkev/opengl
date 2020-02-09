#include "pch.h"
#include "LoadShader.h"

#include <string>
#include <fstream>
#include <vector>

#include <GL/glew.h>

namespace glen
{
	namespace LoadShaders
	{
		static std::string read_shader_code(const char * shader_code);

		static void compile_shader(const GLuint id, const std::string& shader_code);

		static void check_program(const GLuint program_id);

		GLuint load(const char * vertex_file_path, const char * geometry_file_path, const char* fragment_file_path) {

			// // Create the shaders // //
			GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
			GLuint geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
			GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
			

			std::string vertex_shader_code = read_shader_code(vertex_file_path);
			std::string geometry_shader_code = geometry_file_path ? read_shader_code(geometry_file_path) : "";
			std::string fragment_shader_code = read_shader_code(fragment_file_path);

			compile_shader(vertex_shader_id, vertex_shader_code);

			if (geometry_file_path)
			{
				compile_shader(geometry_shader_id, geometry_shader_code);
			}

			compile_shader(fragment_shader_id, fragment_shader_code);

			// Link to program
			GLuint program_id = glCreateProgram();
			glAttachShader(program_id, vertex_shader_id);
			if (geometry_file_path)
			{
				glAttachShader(program_id, geometry_shader_id);
			}
			glAttachShader(program_id, fragment_shader_id);
			glLinkProgram(program_id);

			check_program(program_id);

			glDetachShader(program_id, vertex_shader_id);
			glDetachShader(program_id, fragment_shader_id);

			glDeleteShader(vertex_shader_id);
			glDeleteShader(fragment_shader_id);

			return program_id;
		}

		static std::string read_shader_code(const char* file_path)
		{
			std::string shader_code;
			std::ifstream shader_stream(file_path, std::ios::in);

			if (shader_stream.is_open()) 
			{
				std::string Line = "";
				while (getline(shader_stream, Line))
					shader_code += "\n" + Line;
				shader_stream.close();
			}
			else 
			{
				printf("Impossible to open %s. Are you in the right directory?\n", file_path);
				getchar();
				return 0;
			}
			return shader_code;
		}

		static void compile_shader(const GLuint id, const std::string& shader_code)
		{
			GLint result = GL_FALSE;
			GLint log_length;

			// Compile Shader
			char const * VertexSourcePointer = shader_code.c_str();
			glShaderSource(id, 1, &VertexSourcePointer, NULL);
			glCompileShader(id);

			// Check Shader
			glGetShaderiv(id, GL_COMPILE_STATUS, &result);
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
			if (log_length > 0) 
			{
				std::vector<char> VertexShaderErrorMessage(log_length + 1);
				glGetShaderInfoLog(id, log_length, NULL, &VertexShaderErrorMessage[0]);
				printf("%s\n", &VertexShaderErrorMessage[0]);
			}
		}

		GLuint load(const char * vertex_file_path, const char* fragment_file_path)
		{
			return load(vertex_file_path, NULL, fragment_file_path);
		}

		static void check_program(const GLuint program_id)
		{
			GLint result = GL_FALSE;
			GLint log_length;

			glGetProgramiv(program_id, GL_LINK_STATUS, &result);
			glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
			if (log_length > 0) 
			{
				std::vector<char> ProgramErrorMessage(log_length + 1);
				glGetProgramInfoLog(program_id, log_length, NULL, &ProgramErrorMessage[0]);
				printf("%s\n", &ProgramErrorMessage[0]);
			}
		}
	}
}
