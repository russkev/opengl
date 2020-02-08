#ifndef GL_ENGINE_SHADER_TEXT2D_H
#define GL_ENGINE_SHADER_TEXT2D_H

#include <vector>
#include <tuple>
#include <iostream>

#include <glm/glm.hpp>

#include "Texture.h"
#include "LoadShader.h"

#include "../resources/Buffer.h"
#include "../resources/VAO.h"
#include "../resources/GL_Type_Traits.h"
#include "../resources/GL_Tuple_Introspect.h"

namespace gl_engine_exe
{
	/*
	
		Used for printing 2D text to the opengl screen
	
	*/
	static constexpr auto MAX_LETTERS = 200u;

	struct Text2D
	{
		// // ----- TYPE DEFINITIONS ----- // //
		typedef std::tuple< glm::vec2, glm::vec2, GLuint>	vertex_2d_type;
		typedef std::vector<vertex_2d_type>					vertices_2d_type;

		// // ----- ENUMERATIONS ----- // //
		enum attr { position2D = 0, uv2D = 1 };

		// // ----- CONSTRUCTORS ----- // //
		Text2D(const char* s_texture_path);				// Constructor
		~Text2D();										// Destructor

		// // ----- INIT ----- // //
		void init(int s_x, int s_y, int size, int s_screen_width, int s_screen_height);
		void init_vertices();
		void init_shaders();

		// // ----- GENERAL METHODS ----- // //
		void print(const char* s_text);
		void draw();
		void convert_string(const char* s_text);
		void cleanup();
	
		// // ----- MEMBER VARIABLES ----- // //
	public:
		Texture				m_texture;
		vertices_2d_type	m_vertices;
		int					m_x, m_y, m_size;
		int					m_text_array[MAX_LETTERS];
		GLuint				m_program_id, m_width_uniform_id, m_height_uniform_id, m_string_uniform_id, m_screen_width, m_screen_height;
		Buffer				m_buffer = { GL_ARRAY_BUFFER, 0 };
		VAO					m_vao;
	};
} // namespace gl_engine
#endif