#ifndef GL_ENGINE_MESH_CYLINDER_H
#define GL_ENGINE_MESH_CYLINDER_H

#include "Mesh.h"

namespace glen
{
	/*!
	
		Basic cylinder with cap.

	*/
	struct Cylinder
	{
		// // ----- MAIN ----- // //

		/*!
		 * @return Default cylinder mesh with radius: 0.5, height: 2 and 24 faces around the side.
		*/
		static Mesh create_cylinder();

		/*!
		 * @param radius The radius of the cylinder.
		 * @param height The height of the cylinder.
		 * @return A cylinder mesh with given radius and height and 24 faces around the side.
		*/
		static Mesh create_cylinder(const float radius, const float height);

		/*!
		 * @param radius The radius of the cylinder.
		 * @param height The height of the cylinder.
		 * @param width_segments The number of edges around the side. A higher number will result in a smoother cylinder.
		 * @param height_segments The number of subdivisions going up the height of the cylinder. This will not affect the smoothness of the cylinder.
		 * @return 
		*/
		static Mesh create_cylinder(const float radius, const float height, const uint16_t width_segments, const uint16_t height_segments);

		// // ----- HELPER ----- // //
	private:
		static glm::vec3 radial_to_cartesian(float radius, float theta, float height);
	};
}
#endif