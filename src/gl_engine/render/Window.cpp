#include "pch.h"
#include "Window.h"

namespace glen
{
	// // ----- CONSTRUCTORS ----- // //
	Window::Window(std::string name, GLuint width, GLuint height) :
		Window{ name, create_default_config(), width, height }
	{}

	Window::Window(std::string name, std::vector<OpenGLAttrPair> st_config, GLuint width, GLuint height) :
		m_name(name)
	{
		if (m_st_window) SDL_DestroyWindow(m_st_window);
		if (m_st_opengl) SDL_GL_DeleteContext(m_st_opengl);
		init(st_config, width, height);
	}

	Window::~Window()
	{
		SDL_DestroyWindow(m_st_window);
		SDL_GL_DeleteContext(m_st_opengl);
	}


	void DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		{
			return;
		}

		printf("Open Debug Output message: ");

		if (source == GL_DEBUG_SOURCE_API_ARB)					printf("Source: API ; ");
		else if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)	printf("Source: WINDOW_SYSTEM ; ");
		else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)	printf("Source: SHADER_COMPILER ; ");
		else if (source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)		printf("Source: THIRD_PARTY ; ");
		else if (source == GL_DEBUG_SOURCE_OTHER_ARB)			printf("Source: OTHER ; ");

		if (type == GL_DEBUG_TYPE_ERROR_ARB)					printf("Type: ERROR ; ");
		else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)	printf("Type: DEPRECATED_BEHAVIOUR ; ");
		else if (type == GL_DEBUG_TYPE_PORTABILITY_ARB)			printf("Type: PORTABILITY ; ");
		else if (type == GL_DEBUG_TYPE_PERFORMANCE_ARB)			printf("Type: PERFORMANCE ; ");
		else if (type == GL_DEBUG_TYPE_OTHER_ARB)				printf("Type: OTHER ; ");

		if (severity == GL_DEBUG_SEVERITY_HIGH_ARB)				printf("Severity: HIGH ; ");
		else if (severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)		printf("Severity: MEDIUM ; ");
		else if (severity == GL_DEBUG_SEVERITY_LOW_ARB)			printf("Severity: LOW ; ");


		printf("\nMessage : %s\n", message);
		__debugbreak();
	}

	// // ----- INIT ----- // //
	void Window::init(std::vector<OpenGLAttrPair> st_config, GLuint width, GLuint height)
	{
#ifdef _DEBUG
		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
#endif
		SDL_Init(SDL_INIT_EVERYTHING);
		std::atexit(SDL_Quit);

		for (const auto& it : st_config)
		{
			SDL_GL_SetAttribute(it.key, it.value);
		}

		// // Request a debug context // //
		SDL_GL_SetAttribute(
			SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG
		);

		// // Create window // //
		init_dimensions(&width, &height);


		m_st_window = SDL_CreateWindow("GL Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
		assert(m_st_window != nullptr);


		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		m_st_opengl = SDL_GL_CreateContext(m_st_window);
		if (m_st_opengl == nullptr)
		{
			printf("Error couldn't create GL Context. Possible cause: Open GL version not supported");
			__debugbreak();
		}
		assert(m_st_opengl != nullptr);
		SDL_GL_SetSwapInterval(1);


		// // Initialise GLEW // //
		glewExperimental = true;
		auto loc_glewok = glewInit();
		assert(loc_glewok == GLEW_OK);
		if (GLEW_ARB_debug_output) {
			printf("The OpenGL implementation provides debug output. Let's use it! \n");
#ifdef DEBUG
			glDebugMessageCallbackARB(&DebugOutputCallback, NULL);
#endif
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		}

		print_gl_properties();
	}

	void Window::init_dimensions(GLuint *width, GLuint *height)
	{
		float dpi;
		if (SDL_GetDisplayDPI(0, NULL, &dpi, NULL) < 0)
		{
			printf("SDL_DisplayDPI failed: %s\n", SDL_GetError());
		}
		float scale_factor = dpi / DEFAULT_WINDOWS_DPI;
		*width = (GLuint)(*width * scale_factor);
		*height = (GLuint)(*height * scale_factor);
	}

	// // ----- GENERAL METHODS ----- // //
	void Window::finish_frame()
	{
		SDL_GL_SwapWindow(m_st_window);
	}

	void Window::append_title(std::string str/*const char* title*/)
	{
		std::string concat_title_str = (m_name + " | " + str);
		char concat_title_char[256];
		strncpy_s(concat_title_char, concat_title_str.c_str(), sizeof(concat_title_char));

		SDL_SetWindowTitle(m_st_window, concat_title_char);
	}

	void Window::print_gl_properties()
	{
		printf("OpenGL loaded\n");
		printf("Vendor: %s\n", glGetString(GL_VENDOR));
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		printf("Version: %s\n", glGetString(GL_VERSION));
	}

	std::vector<OpenGLAttrPair> Window::create_default_config()
	{
		return
		{
			{SDL_GL_RED_SIZE,					8},
			{SDL_GL_GREEN_SIZE,					8},
			{SDL_GL_BLUE_SIZE,					8},
			{SDL_GL_ALPHA_SIZE,					8},
			{SDL_GL_BUFFER_SIZE,				32},
			{SDL_GL_DOUBLEBUFFER,				1},
			{SDL_GL_DEPTH_SIZE,					24},
			{SDL_GL_STENCIL_SIZE,				8},
			{SDL_GL_ACCUM_RED_SIZE,				0},
			{SDL_GL_ACCUM_GREEN_SIZE,			0},
			{SDL_GL_ACCUM_BLUE_SIZE,			0},
			{SDL_GL_ACCUM_ALPHA_SIZE,			0},
			{SDL_GL_STEREO,						0},
			{SDL_GL_MULTISAMPLEBUFFERS,			1},
			{SDL_GL_MULTISAMPLESAMPLES,			4},
			{SDL_GL_ACCELERATED_VISUAL,			1},
			{SDL_GL_CONTEXT_MAJOR_VERSION,		4},
			{SDL_GL_CONTEXT_MINOR_VERSION,		4},
			{SDL_GL_CONTEXT_EGL,				0},
			{SDL_GL_CONTEXT_FLAGS,				SDL_GL_CONTEXT_DEBUG_FLAG},
			{SDL_GL_CONTEXT_PROFILE_MASK,		SDL_GL_CONTEXT_PROFILE_CORE},
			{SDL_GL_SHARE_WITH_CURRENT_CONTEXT,	1},
			{SDL_GL_FRAMEBUFFER_SRGB_CAPABLE,	1}
		};
	}


	// // ----- GETTERS ----- // //
	GLint Window::width()
	{
		GLint width;
		SDL_GetWindowSize(m_st_window, &width, NULL);
		return width;
	}

	GLint Window::height()
	{
		GLint height;
		SDL_GetWindowSize(m_st_window, NULL, &height);
		return height;
	}
}