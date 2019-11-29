#include <math.h>

#include "../ShapeData.h"

struct Sphere
{
	static ShapeData createSphere(float radius, uint32_t widthSegments, uint32_t heightSegments);
};