#ifndef GL_ENGINE_MESH_LIBRARY_H
#define GL_ENGINE_MESH_LIBRARY_H

#include <glm\glm.hpp>

#include "Mesh.h"

namespace glen
{
	/*

	Basic cube.
	Can instantiate with an overall size which is the size of each of the edges.

	*/
	struct Cube
	{
		// // ----- MAIN ----- // //
		static Mesh create_cube();
		static Mesh create_cube(const float width, const bool is_default = false);

		// // ----- HELPER ----- // //
	private:
		static Mesh create_cube_face(glm::vec3 top_left, glm::vec3 top_right, glm::vec3 bottom_left, glm::vec3 bottom_right);
	};
}

#endif