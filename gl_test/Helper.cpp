#include "pch.h"
#include "Helper.h"


namespace Helper
{
	bool Helper::matrix_is_similar(const glm::mat4& result, const glm::mat4& expected, float tolerance)
	{
		bool is_similar = true;
		for (auto i = 0; i < 4; ++i)
		{
			for (auto j = 0; j < 4; ++j)
			{
				if ((std::abs(result[i][j] - expected[i][j])) > tolerance)
				{
					is_similar = false;
				}
			}
		}
		return is_similar;
	}


	//GLint Helper::check_binding(const GLenum target, const GLuint id)
	//{
	//	glBindVertexArray(id);
	//	GLint which_id;
	//	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &which_id);
	//	return which_id;
	//}
}