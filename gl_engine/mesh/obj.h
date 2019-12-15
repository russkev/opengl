#ifndef GL_ENGINE_MESH_OBJ_H
#define GL_ENGINE_MESH_OBJ_H

#include "Mesh.h"

#include <stdexcept>
#include <glm/matrix.hpp>
#include <cassert>

namespace gl_engine
{
	struct OBJ_Loader
	{
		// Set constants
		const static glm::vec3 DEFAULT_COLOUR;

		// Static methods
		static Mesh load_obj(const char *path);

	};
} // namespace gl_engine
#endif