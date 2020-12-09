#ifndef GL_ENGINE_SHADER_TEXT2D_H
#define GL_ENGINE_SHADER_TEXT2D_H

#include "Texture.h"
#include "LoadShader.h"
#include "resources/Buffer.h"
#include "resources/VAO.h"
#include "resources/GL_Type_Traits.h"
#include "resources/GL_Tuple_Introspect.h"

namespace glen
{
	static constexpr auto MAX_LETTERS = 200u;

	/*!
	
		Used for printing 2D text to the opengl screen
	
	*/
	struct Text2D
	{
		// @cond

		// // ----- TYPE DEFINITIONS ----- // //
		typedef std::tuple< glm::vec2, glm::vec2, GLuint>	vertex_2d_type;
		typedef std::vector<vertex_2d_type>					vertices_2d_type;

		// // ----- ENUMERATIONS ----- // //
		enum attr { position2D = 0, uv2D = 1 };

		// @endcond

		// // ----- CONSTRUCTORS ----- // //
		
		/*!
		 * @brief Constructor.
		 * @param s_texture_path Path to an image file which contains appropriately spaced letters.
		*/
		Text2D(const char* s_texture_path);				
		~Text2D();										

		// // ----- INIT ----- // //

		/*!
		 * @brief Initialize.
		 * @param s_x The start screen x coordinate
		 * @param s_y The start screen y coordinate
		 * @param size Size of the letters.
		 * @param s_screen_width Screen width
		 * @param s_screen_height Screen height
		*/
		void init(int s_x, int s_y, int size, int s_screen_width, int s_screen_height);
	private:
		void init_vertices();
		void init_shaders();

		// // ----- GENERAL METHODS ----- // //
	public:

		/*!
		 * @brief Print `s_text` to the window.
		*/
		void print(const char* s_text);

		/*!
		 * @brief Render existing text to the window.
		*/
		void draw();

		/*!
		 * @brief Convert `s_text` to a renderable mesh.
		*/
		void convert_string(const char* s_text);
	
		// @cond

		// // ----- MEMBER VARIABLES ----- // //
	public:
		Texture				m_texture{ GL_TEXTURE_2D };
		vertices_2d_type	m_vertices;
		GLint				m_x = 0, m_y = 0, m_size = 0;
		GLint				m_text_array[MAX_LETTERS] = { 0 };
		GLuint				m_program_id = 0, m_width_uniform_id = 0, m_height_uniform_id = 0, m_string_uniform_id = 0, m_screen_width = 0, m_screen_height = 0;
		Buffer				m_buffer = { GL_ARRAY_BUFFER, 0 };
		VAO					m_vao;

		// @endcond
	};
}
#endif