#ifndef GL_ENGINE_MESH_WINDOW_QUAD_H
#define GL_ENGINE_MESH_WINDOW_QUAD_H

#include "Mesh.h"

namespace glen
{
	/*

	Mesh for rendering a 2D image directly to the window

	*/
	struct WindowQuad
	{
		// // ------ MAIN ----- // //
		static Mesh create_windowQuad();

	};
}
#endif