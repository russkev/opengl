#ifndef GL_ENGINE_TEST_HELPER_H
#define GL_ENGINE_TEST_HELPER_H

#include <glm/glm.hpp>
#include "../gl_engine/render/Window.h"

namespace Helper
{
	bool matrix_is_similar(const glm::mat4& result, const glm::mat4& expected, float tolerance);

	//GLint check_binding(const GLenum target, const GLenum binding_name, const GLuint id);
}
#endif
