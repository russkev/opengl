#pragma once

#include "Mesh.h"

struct Plane
{
	static ShapeData createPlane();
	static ShapeData createPlane(const float width, const float height);
	static ShapeData createPlane(const float width, const float height, const uint16_t widthSegments, const uint16_t heightSegments);
};