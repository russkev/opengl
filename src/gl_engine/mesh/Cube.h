#ifndef GL_ENGINE_MESH_LIBRARY_H
#define GL_ENGINE_MESH_LIBRARY_H

#include "Mesh.h"

namespace glen
{
	/*!

		Basic cube.

	*/
	struct Cube
	{
		// // ----- MAIN ----- // //

		/*!
		 * @brief Default cube mesh
		 * @return A cube that goes from -1 to 1 in x, y and z.
		*/
		static Mesh create_cube();

		/*!
		 * @param width The width of all sides of the cube
		 * @return A cube mesh that sits on the ground plane
		*/
		static Mesh create_cube(const float width, const bool is_default = false);

		// // ----- HELPER ----- // //
	private:
		static Mesh create_cube_face(glm::vec3 top_left, glm::vec3 top_right, glm::vec3 bottom_left, glm::vec3 bottom_right);
	};
}

#endif