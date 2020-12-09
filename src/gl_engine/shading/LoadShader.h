#ifndef GL_ENGINE_SHADING_LOAD_SHADER_H
#define GL_ENGINE_SHADING_LOAD_SHADER_H

namespace glen
{
	/*!
	
		Used for making a shader program from a vertex and fragment shader.
	
	*/
	namespace LoadShaders
	{
		// // ----- MAIN ----- // //
		
		/*!
		 * @brief Construct a shader program from a vertex and fragment shader
		 * @return The ID of the newly created shader program.
		*/
		GLuint load(const char * vertex_file_path, const char* fragment_file_path);

		/*!
		 * @brief Construct a shader program from a vertex, geometry and fragment shader
		 * @return The ID of the newly created shader program.
		*/
		GLuint load(const char * vertex_file_path, const char* geometry_file_path, const char* fragment_file_path);
	}
}
#endif