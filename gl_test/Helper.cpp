#include "pch.h"
#include "Helper.h"


namespace Helper
{
	bool Helper::matrix_is_similar(const glm::mat4& result, const glm::mat4& expected, float tolerance)
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


	glen::Window create_gl_window()
	{

		glm::uvec2 dimensions{ 512, 512 };

		glen::Window window{ "GL Test Window", dimensions.x, dimensions.y };

		return window;
	}
}