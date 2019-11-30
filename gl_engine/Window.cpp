#include "Window.h"

Window::Window()
{
	if (m_st_window) SDL_DestroyWindow(m_st_window);
	if (m_st_opengl) SDL_GL_DeleteContext(m_st_opengl);
}


void DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
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
	else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		printf("Severity: NOTIFICATION ; ");
		printf("\nMessage : %s\n", message);
		return;
	}

	printf("\nMessage : %s\n", message);
	__debugbreak();
}

void Window::init(std::vector<opengl_attr_pair> st_config, GLuint width, GLuint height)
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
	m_st_window = SDL_CreateWindow("Tutorial 04 - A Cloured Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	assert(m_st_window != nullptr);

	
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	m_st_opengl = SDL_GL_CreateContext(m_st_window);
	if (m_st_opengl == nullptr)
	{
		printf("Error couldn't create GL Context. Possible cause: Open GL version available is too low");
		__debugbreak();
	}
	//assert(m_st_opengl != nullptr);

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

}

void Window::finish_frame()
{
	SDL_GL_SwapWindow(m_st_window);
}