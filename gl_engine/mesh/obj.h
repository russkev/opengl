#ifndef GL_ENGINE_MESH_OBJ_H
#define GL_ENGINE_MESH_OBJ_H

#include "Mesh.h"

#include <stdexcept>
#include <glm/matrix.hpp>
#include <cassert>

namespace gl_engine
{
	/*
	
		OBJ Loader.
		At the moment, the obj files have to be in triangulated form.
	
	*/
	struct OBJ_Loader
	{
		// // ------ CONSTANTS ----- // //
		const static glm::vec3 DEFAULT_COLOUR;

		// // ------ MAIN ----- // //
		static Mesh load_obj(const char *path);

	};
} // namespace gl_engine
#endif