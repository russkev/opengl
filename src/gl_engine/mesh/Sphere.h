#ifndef GL_ENGINE_MESH_SPHERE_H
#define GL_ENGINE_MESH_SPHERE_H

#include "Mesh.h"

namespace glen
{
	/*!

		Basic sphere

	*/
	struct Sphere
	{
		// // ------ MAIN ----- // //

		/*!
		 * @return Sphere mesh of radius `radius`.
		*/
		static Mesh create_sphere(const float radius);

		/*!
		 * @return Sphere mesh with radius `radius` and `segments` edges around and `segments/2` edges high. This ratio generally looks even.
		*/
		static Mesh create_sphere(const float radius, const uint16_t segments);

		/*!
		 * @return Sphere mesh with radius `radius` and `width_segments` edges around and `height_segments` edges high.
		*/
		static Mesh create_sphere(const float radius, const uint16_t width_segments, const uint16_t height_segments);
	private:
		// // ------ HELPER ----- // //
		static glm::vec3 spherical_to_cartesian(const float radius, const float theta, const float phi);
	};
}
#endif