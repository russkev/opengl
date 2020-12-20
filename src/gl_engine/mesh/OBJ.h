#ifndef GL_ENGINE_MESH_OBJ_H
#define GL_ENGINE_MESH_OBJ_H

#include "Mesh.h"

namespace glen
{
	/*!

		OBJ Loader.
		At the moment, the obj files have to be in triangulated form.

	*/
	struct OBJ_Loader
	{
		// // ------ CONSTANTS ----- // //
		// @cond

		const static glm::vec3 DEFAULT_COLOUR;

		// @endcond

		// // ------ MAIN ----- // //

		/*!
		 * @param path The path to the OBJ file
		 * @return The new mesh
		*/
		static Mesh load_obj(const char *path);

	};
}

#endif