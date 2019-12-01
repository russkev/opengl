#pragma once

#include <math.h>

#include "Mesh.h"

struct Sphere
{
	static Mesh createSphere(const float radius);
	static Mesh createSphere(const float radius, const uint16_t segments);
	static Mesh createSphere(const float radius, const uint16_t widthSegments, const uint16_t heightSegments);
private:
	static glm::vec3 sphericalToCartesian(const float radius, const float theta, const float phi);
};