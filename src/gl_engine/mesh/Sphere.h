#ifndef GL_ENGINE_MESH_SPHERE_H
#define GL_ENGINE_MESH_SPHERE_H

#include "Mesh.h"

namespace glen
{
	/*

	Basic sphere

	*/
	struct Sphere
	{
		// // ------ MAIN ----- // //
		static Mesh create_sphere(const float radius);
		static Mesh create_sphere(const float radius, const uint16_t segments);
		static Mesh create_sphere(const float radius, const uint16_t width_segments, const uint16_t height_segments);
	private:
		// // ------ HELPER ----- // //
		static glm::vec3 spherical_to_cartesian(const float radius, const float theta, const float phi);
	};
}
#endif