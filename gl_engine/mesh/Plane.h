#ifndef GL_ENGINE_MESH_PLANE_H
#define GL_ENGINE_MESH_PLANE_H

#include "Mesh.h"

namespace gl_engine
{
	/*
	
		Basic plane

	*/
	struct Plane
	{
		static Mesh createPlane();
		static Mesh createPlane(const float width, const float height);
		static Mesh createPlane(const float width, const float height, const uint16_t widthSegments, const uint16_t heightSegments);
	};
} // namespace gl_engine
#endif