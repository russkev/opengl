#ifndef GL_ENGINE_MESH_CUBE_H
#define GL_ENGINE_MESH_CUBE_H

#include "Mesh.h"

namespace gl_engine
{
	/*
	
		Basic cube.
		Can instantiate with an overall size which is the size of each of the edges.
	
	*/
	struct Cube
	{
		static Mesh create_cube();
		static Mesh create_cube(const float width);
	private:
		static Mesh create_cube_face(glm::vec3 top_left, glm::vec3 top_right, glm::vec3 bottom_left, glm::vec3 bottom_right);
	};
} // namespace gl_engine
#endif