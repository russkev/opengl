#include <cassert>

#include "Plane.h"
#include "../Vertex.h"

ShapeData Plane::createPlane()
{
	return createPlane(1.0f, 1.0f, 1, 1);
}

ShapeData Plane::createPlane(const float width, const float height)
{
	return createPlane(width, height, 1, 1);
}

ShapeData Plane::createPlane(const float width, const float height, const uint16_t widthSegments, const uint16_t heightSegments)
{
	assert(width > 0);
	assert(height > 0);
	assert(widthSegments > 0);
	assert(heightSegments > 0);

	ShapeData newPlane;
	float widthOffset = width / (float)widthSegments;
	float heightOffset = height / (float)heightSegments;
	float widthUVOffset = 1 / (float)widthSegments;
	float heightUVOffset = 1 / (float)heightSegments;
	float startX = width * -0.5f;
	float startZ = height * -0.5f;
	glm::vec3 normal = { 0.0, 1.0, 0.0 };

	for(uint16_t x = 0; x < widthSegments; ++x)
	{
		for (uint16_t z = 0; z < heightSegments; ++z)
		{
			// Vertix order is like this:
			//
			//		  v1-----v2
			//		   |     |
			//		   |     |
			//		  v3-----v4
			//

			// Vertex positions
			glm::vec3 v1 = { startX + widthOffset * (float)(x),		0.0f,	startZ + heightOffset * (float)(z) };
			glm::vec3 v2 = { startX + widthOffset * (float)(x + 1),	0.0f,	startZ + heightOffset * (float)(z) };
			glm::vec3 v3 = { startX + widthOffset * (float)(x),		0.0f,	startZ + heightOffset * (float)(z + 1) };
			glm::vec3 v4 = { startX + widthOffset * (float)(x + 1),	0.0f,	startZ + heightOffset * (float)(z + 1) };

			// Vertex UVs
			glm::vec2 uv1 = { widthUVOffset * (float)(x),		heightUVOffset * (float)(heightSegments - z) };
			glm::vec2 uv2 = { widthUVOffset * (float)(x + 1),	heightUVOffset * (float)(heightSegments - z) };
			glm::vec2 uv3 = { widthUVOffset * (float)(x),		heightUVOffset * (float)(heightSegments - z - 1) };
			glm::vec2 uv4 = { widthUVOffset * (float)(x + 1),	heightUVOffset * (float)(heightSegments - z - 1) };

			// Vertices
			Vertex vert1 = { v1, normal, uv1 };
			Vertex vert2 = { v2, normal, uv2 };
			Vertex vert3 = { v3, normal, uv3 };
			Vertex vert4 = { v4, normal, uv4 };

			newPlane.appendTriangle(vert1, vert3, vert2);
			newPlane.appendTriangle(vert3, vert4, vert2);
		}
	}
	newPlane.makeIndicesSmooth();
	newPlane.makeTangents();
	return newPlane;
}