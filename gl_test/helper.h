#pragma once

#include <glm/glm.hpp>

namespace Helper
{
	bool matrixIsSimilar(glm::mat4 result, glm::mat4 expected, float tolerance)
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
}
