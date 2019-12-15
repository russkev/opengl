#ifndef GL_ENGINE_MESH_SPHERE_H
#define GL_ENGINE_MESH_SPHERE_H

#include <math.h>

#include "Mesh.h"

namespace gl_engine
{
	struct Sphere
	{
		static Mesh createSphere(const float radius);
		static Mesh createSphere(const float radius, const uint16_t segments);
		static Mesh createSphere(const float radius, const uint16_t widthSegments, const uint16_t heightSegments);
	private:
		static glm::vec3 sphericalToCartesian(const float radius, const float theta, const float phi);
	};
} // namespace gl_engine
#endif