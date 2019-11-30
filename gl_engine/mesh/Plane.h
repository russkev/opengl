#pragma once

#include "Mesh.h"

struct Plane
{
	static Mesh createPlane();
	static Mesh createPlane(const float width, const float height);
	static Mesh createPlane(const float width, const float height, const uint16_t widthSegments, const uint16_t heightSegments);
};