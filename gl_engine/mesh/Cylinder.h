#pragma once

#include "Mesh.h"

struct Cylinder
{
	static Mesh createCylinder();
	static Mesh createCylinder(const float radius, const float height);
	static Mesh createCylinder(const float radius, const float height, const uint16_t widthSegments, const uint16_t heightSegments);
private:
	static glm::vec3 radialToCartesian(float radius, float theta, float height);
};