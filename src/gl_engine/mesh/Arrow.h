#ifndef GL_ENGINE_MESH_ARROW_H
#define GL_ENGINE_MESH_ARROW_H

#include "Mesh.h"

namespace glen
{
	/*!

		Basic 3D arrow shape.

	*/
	struct Arrow
	{
		// // ----- MAIN ----- // //
		
		/*!
		 * @return 3D arrow mesh.
		*/
		static Mesh create_arrow();

		/*!
		 * @param length The length of the tail.
		 * @return 3D arrow mesh.
		*/
		static Mesh create_arrow(const float length);

		// // ----- HELPER ----- // //
	private:
		static Mesh create_rectangle_face(glm::vec3 tl, glm::vec3 tr, glm::vec3 bl, glm::vec3 br);
		static Mesh create_arrow_face(glm::vec3 t, glm::vec3 cl, glm::vec3 cr, glm::vec3 il, glm::vec3 ir, glm::vec3 bl, glm::vec3 br);

	};
}

#endif