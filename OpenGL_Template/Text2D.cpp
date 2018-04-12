#include "Text2D.h"

// // CONSTRUCTOR
Text2D::Text2D(const char* s_texture_path):
	m_texture(Texture(s_texture_path)) 
{};

void Text2D::init(int s_x, int s_y, int s_size, int s_screen_width, int s_screen_height)
{
	m_x = s_x;
	m_y = s_y;
	m_size = s_size;
	m_screen_width = s_screen_width;
	m_screen_height = s_screen_height;
	initVertices();
	initShaders();
}

void Text2D::print(const char* s_text)
{
	int* charArray = string_to_array(s_text);
	//printToConsole(charArray);
	glUseProgram(m_program_id);
	glUniform1i(m_width_uniform_id, m_screen_width);
	glUniform1i(m_height_uniform_id, m_screen_height);

	//unsigned int max_letters = 200;
	char stringToRender[MAX_LETTERS];
	int test_string[] = { 'A', 'r', 's', 'e' };
	//int test_string[] = string_to_array(s_text);


	glUniform1iv(m_string_uniform_id, MAX_LETTERS, charArray);
	m_texture.upload_to_shader(m_program_id, "fontTexture", 0);

	m_buffer.Append(m_vertices);
	static const auto text2D_info = gl_introspect_tuple<std::tuple<glm::vec2, glm::vec2, GLuint>>::get();
	m_vao.GenerateVAO(m_buffer, 0, text2D_info.data(), text2D_info.data() + text2D_info.size());

	auto c = 1;
}

void Text2D::draw()
{
	glUseProgram(m_program_id);
	m_vao.Bind();
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_buffer.size());
}

void Text2D::initVertices()
{
	unsigned int offset = 32;
	unsigned int num_points = 4;
	for (unsigned int i = 0; i < MAX_LETTERS; ++i)
	{
		char character = char(32);
		float uv_size = 1.0f / 16.0f;
		float uv_x = (character % 16) * uv_size;
		float uv_y = (character / 16) * uv_size;

		vertex2DType top_left = { { m_x + i * m_size,			m_y + m_size },{ uv_x,				1.0 - uv_y }, i*num_points + 0 };
		vertex2DType top_right = { { m_x + i * m_size + m_size,	m_y + m_size },{ uv_x + uv_size,	1.0 - uv_y }, i*num_points + 1 };
		vertex2DType bottom_left = { { m_x + i * m_size,			m_y },{ uv_x,				1.0 - uv_y - uv_size }, i*num_points + 2 };
		vertex2DType bottom_right = { { m_x + i * m_size + m_size,	m_y },{ uv_x + uv_size,	1.0 - uv_y - uv_size }, i*num_points + 3 };

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

void Text2D::initShaders()
{
	m_program_id = LoadShaders("Text2d.vert", "Text2D.frag");
	glUseProgram(m_program_id);
	m_width_uniform_id = glGetUniformLocation(m_program_id, "width");
	m_height_uniform_id = glGetUniformLocation(m_program_id, "height");
	m_string_uniform_id = glGetUniformLocation(m_program_id, "text_string");
}

int* Text2D::string_to_array(const char* s_text)
{
	auto space					= 96;
	bool end					= false;
	int charArray[MAX_LETTERS];

	
	for (auto i = 0; i < MAX_LETTERS; ++i)
	{
		auto letter = (int)s_text[i];
		if (s_text[i] == *"\0")	end = true;
		end == false ? charArray[i] = letter : charArray[i] = space;	
	}
	return charArray;
}

void Text2D::printToConsole(const int* charArray)
{
	std::cout << charArray[4] << "\n";
	std::cout << charArray[5] << "\n";
	//for (auto i = 0; i < MAX_LETTERS; ++i)
	//{
	//	std::cout << charArray[i] << ", ";
	//}
}

void Text2D::cleanup()
{
	//
}