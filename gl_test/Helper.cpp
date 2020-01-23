#include "pch.h"
#include "Helper.h"


namespace Helper
{
	bool Helper::matrix_is_similar(glm::mat4 result, glm::mat4 expected, float tolerance)
	{
		bool isSimilar = true;
		for (auto i = 0; i < 4; ++i)
		{
			for (auto j = 0; j < 4; ++j)
			{
				if ((std::abs(result[i][j] - expected[i][j])) > tolerance)
				{
					isSimilar = false;
				}
			}
		}
		return isSimilar;
	}


	gl_engine::Window create_gl_window()
	{
		std::vector<gl_engine::opengl_attr_pair> st_config =
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

		glm::uvec2 dimensions{ 512, 512 };

		gl_engine::Window window{ "GL Test Window", st_config, dimensions.x, dimensions.y };

		return window;
	}
}