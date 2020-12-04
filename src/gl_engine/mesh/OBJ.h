#ifndef GL_ENGINE_MESH_OBJ_H
#define GL_ENGINE_MESH_OBJ_H

#include <glm/glm.hpp>

#include "Mesh.h"

namespace glen
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
}

#endif