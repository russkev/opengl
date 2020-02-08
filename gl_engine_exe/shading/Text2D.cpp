#include "Text2D.h"

namespace gl_engine_exe
{
	// // ----- CONSTRUCTORS ----- // //
	Text2D::Text2D(const char* s_texture_path) :
		m_texture(Texture(s_texture_path))
	{};

	// Destructor
	Text2D::~Text2D()
	{
		glUseProgram(0);
		glDeleteProgram(m_program_id);
	}

	// // ----- INIT ----- // //
	void Text2D::init(int s_x, int s_y, int s_size, int s_screen_width, int s_screen_height)
	{
		m_x = s_x;
		m_y = s_y;
		m_size = s_size;
		m_screen_width = s_screen_width;
		m_screen_height = s_screen_height;
		init_vertices();
		init_shaders();
	}

	void Text2D::init_vertices()
	{
		unsigned int offset = 32;
		unsigned int num_points = 4;
		for (unsigned int i = 0; i < MAX_LETTERS; ++i)
		{
			char character = char(32);
			float uv_size = 1.0f / 16.0f;
			float uv_x = (character % 16) * uv_size;
			float uv_y = (character / 16) * uv_size;

			vertex_2d_type top_left = { { m_x + i * m_size,			m_y + m_size },{ uv_x,				1.0 - uv_y }, i*num_points + 0 };
			vertex_2d_type top_right = { { m_x + i * m_size + m_size,	m_y + m_size },{ uv_x + uv_size,	1.0 - uv_y }, i*num_points + 1 };
			vertex_2d_type bottom_left = { { m_x + i * m_size,			m_y },{ uv_x,				1.0 - uv_y - uv_size }, i*num_points + 2 };
			vertex_2d_type bottom_right = { { m_x + i * m_size + m_size,	m_y },{ uv_x + uv_size,	1.0 - uv_y - uv_size }, i*num_points + 3 };

			m_vertices.push_back(top_left);
			m_vertices.push_back(bottom_left);
			m_vertices.push_back(top_right);

			m_vertices.push_back(top_right);
			m_vertices.push_back(bottom_left);
			m_vertices.push_back(bottom_right);

			auto test = m_x;
			auto x = 0;
		}
	}

	void Text2D::init_shaders()
	{
		m_program_id = LoadShaders::load("Text2d.vert", "Text2D.frag");
		glUseProgram(m_program_id);
		m_width_uniform_id = glGetUniformLocation(m_program_id, "width");
		m_height_uniform_id = glGetUniformLocation(m_program_id, "height");
		m_string_uniform_id = glGetUniformLocation(m_program_id, "text_string");
	}


	// // ----- GENERAL METHODS ----- // //
	void Text2D::print(const char* s_text)
	{
		glUseProgram(m_program_id);
		//m_texture.upload_to_shader(m_program_id, "fontTexture");
		convert_string(s_text);

		glUniform1i(m_width_uniform_id, m_screen_width);
		glUniform1i(m_height_uniform_id, m_screen_height);
		glUniform1iv(m_string_uniform_id, MAX_LETTERS, m_text_array);
		m_buffer.bind();
		m_buffer.append(m_vertices);
		static const auto text2D_info = gl_introspect_tuple<std::tuple<glm::vec2, glm::vec2, GLuint>>::get();
		m_vao.generate_VAO(m_buffer, 0, text2D_info.data(), text2D_info.data() + text2D_info.size());
		glUseProgram(0);
	}

	void Text2D::draw()
	{
		glUseProgram(m_program_id);
		m_vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_buffer.size());
	}

	void Text2D::convert_string(const char* s_text)
	{
		auto space = 32;
		bool end = false;

		for (auto i = 0; i < MAX_LETTERS; ++i)
		{
			auto letter = (int)s_text[i];
			if (s_text[i] == *"\0")	end = true;
			end == false ? m_text_array[i] = letter : m_text_array[i] = space;
		}
		return;
	}

	void Text2D::cleanup()
	{
		//
	}
} // namespace gl_engine