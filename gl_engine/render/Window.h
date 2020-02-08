#ifndef GL_ENGINE_WINDOW_H
#define GL_ENGINE_WINDOW_H

#include <vector>
#include <string>


#include <SDL.h>
#include <GL/glew.h>


namespace glen
{
	struct OpenGLAttrPair
	{
		SDL_GLattr key;
		int value;
	};

	struct Window
	{
		// // ----- CONSTANTS ----- // //
		static constexpr float DEFAULT_WINDOWS_DPI = 96.0f;

		// // ----- CONSTRUCTORS ----- // //
		Window(std::string name, GLuint width, GLuint height);
		Window(std::string name, std::vector<OpenGLAttrPair> st_config, GLuint width, GLuint height);

		// // ----- INIT ----- // //
		void init(std::vector<OpenGLAttrPair> st_config, GLuint width, GLuint height);

	//	// // ----- GENERAL METHODS ----- // //
		void append_title(std::string str);
		void finish_frame();
		std::vector<OpenGLAttrPair> create_default_config();
	private:
		void init_dimensions(GLuint* width, GLuint* height);
		void print_gl_properties();

		// // ----- GETTERS ----- // //
	public:
		GLint width();
		GLint height();



		// // ----- MEMBER VARIABLES ----- // //
	public:
		SDL_Window *	m_st_window = nullptr;
		SDL_GLContext	m_st_opengl = nullptr;
	private:
		std::string m_name = "";
	};
}
#endif