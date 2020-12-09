#ifndef GL_ENGINE_WINDOW_H
#define GL_ENGINE_WINDOW_H

namespace glen
{
	struct OpenGLAttrPair
	{
		SDL_GLattr key;
		int value;
	};

	/*!
	
		A Window object sets up an OpenGL environment for all other components.

		A new window must be constructed before any other components are added to the scene.

	*/
	struct Window
	{
		// @cond

		// // ----- CONSTANTS ----- // //
		static constexpr float DEFAULT_WINDOWS_DPI = 96.0f;

		// @endcond

		// // ----- CONSTRUCTORS ----- // //

		/*!
		 * @brief Constructor
		 * @param name The window name.
		 * @param width Window width in pixels.
		 * @param height Window height in pixels.
		*/
		Window(std::string name, GLuint width, GLuint height);
		
		/*!
		 * @brief Constructor
		 * @param name The window name.
		 * @param stconfig A vector of key value pairs used to configure the OpenGL context.
		 * @param width Window width in pixels.
		 * @param height Window height in pixels.
		*/
		Window(std::string name, std::vector<OpenGLAttrPair> st_config, GLuint width, GLuint height);
		~Window();

		// // ----- INIT ----- // //
	private:
		void init(std::vector<OpenGLAttrPair> st_config, GLuint width, GLuint height);

	//	// // ----- GENERAL METHODS ----- // //
	public:
		/*!
		 * @param str Add this string to the window name.
		 * 
		 * The window title will appear as <Window name> | `str`
		*/
		void append_title(std::string str);

		/*!
		 * @brief Swap the old frame with the newly created frame.
		*/
		void finish_frame();

		/*!
		 * @return A vector of key value pairs that can be used to configure a new `Window`.
		 * 
		 * This can be used to construct a new `Window`.
		*/
		static std::vector<OpenGLAttrPair> create_default_config();
	private:
		void init_dimensions(GLuint* width, GLuint* height);
		void print_gl_properties();

		// // ----- GETTERS ----- // //
	public:
		/*!
		 * @return The window width in pixels.
		*/
		GLint width();

		/*!
		 * @return The window height in pixels.
		*/
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