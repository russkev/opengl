#ifndef GL_ENGINE_MESH_CYLINDER_H
#define GL_ENGINE_MESH_CYLINDER_H

#include "Mesh.h"

namespace gl_engine_exe
{
	/*
	
		Basic cylinder with cap.
		Radius, height, number of height segments, and number of width segments can be defined.
	
	*/
	struct Cylinder
	{
		// // ----- MAIN ----- // //
		static Mesh create_cylinder();
		static Mesh create_cylinder(const float radius, const float height);
		static Mesh create_cylinder(const float radius, const float height, const uint16_t width_segments, const uint16_t height_segments);
		
		// // ----- HELPER ----- // //
	private:
		static glm::vec3 radial_to_cartesian(float radius, float theta, float height);
	};
} // namespace gl_engine
#endif