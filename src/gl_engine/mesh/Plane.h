#ifndef GL_ENGINE_MESH_PLANE_H
#define GL_ENGINE_MESH_PLANE_H

#include "Mesh.h"

namespace glen
{
	/*!

		Basic plane

	*/
	struct Plane
	{
		// // ------ MAIN ----- // //

		/*!
		 * @return A new 1 x 1 plane
		*/
		static Mesh create_plane();

		/*!
		 * @return A new plane of size `width` x `height`.
		*/
		static Mesh create_plane(const float width, const float height);

		/*!
		 * @return A new plane of size `width` x `height`, with `width_segments` edges making up the width and `height_segments` edges making up the height.
		*/
		static Mesh create_plane(const float width, const float height, const uint16_t width_segments, const uint16_t height_segments);
	};
}
#endif