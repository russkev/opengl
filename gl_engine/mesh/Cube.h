#ifndef GL_ENGINE_MESH_CUBE_H
#define GL_ENGINE_MESH_CUBE_H

#include "Mesh.h"

namespace gl_engine
{
	struct Cube
	{
		static Mesh createCube();
		static Mesh createCube(const float width);
	private:
		static Mesh createCubeFace(glm::vec3 tl, glm::vec3 tr, glm::vec3 bl, glm::vec3 br);
	};
} // namespace gl_engine
#endif