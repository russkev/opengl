#ifndef GL_ENGINE_MESH_PLANE_H
#define GL_ENGINE_MESH_PLANE_H

#include "Mesh.h"

namespace glen
{
	/*

	Basic plane

	*/
	struct Plane
	{
		// // ------ MAIN ----- // //
		static Mesh create_plane();
		static Mesh create_plane(const float width, const float height);
		static Mesh create_plane(const float width, const float height, const uint16_t width_segments, const uint16_t height_segments);
	};
}
#endif