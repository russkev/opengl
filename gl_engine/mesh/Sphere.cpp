#define _USE_MATH_DEFINES

#include <cmath>
#include <cassert>

#include "Sphere.h"
#include "Vertex.h"

Mesh Sphere::createSphere(const float radius)
{
	return createSphere(radius, 32);
}

Mesh Sphere::createSphere(const float radius, const uint16_t segments)
{
	return createSphere(radius, segments, segments/2);
}

Mesh Sphere::createSphere(const float radius, const uint16_t widthSegments, const uint16_t heightSegments)
{
	assert (radius > 0);
	assert (widthSegments > 1);
	assert (heightSegments > 1);
	Mesh newSphere;
	// Thanks to user bobobobo on stackexchange
	// Source found here: 
	// https://gamedev.stackexchange.com/questions/16585/how-do-you-programmatically-generate-a-sphere
	//
	for (uint16_t i = 0; i < heightSegments; i++)
	{
		float theta1 = ((float)(i)	   / (float)heightSegments) * (float)M_PI;
		float theta2 = ((float)(i + 1) / (float)heightSegments) * (float)M_PI;

		for (uint16_t j = 0; j < widthSegments; ++j)
		{
			float phi1 = ((float)(j)	 / (float)widthSegments) * 2 * (float)M_PI;
			float phi2 = ((float)(j + 1) / (float)widthSegments) * 2 * (float)M_PI;

			// Vertex positions
			glm::vec3 v1 = sphericalToCartesian(radius, theta1, phi1);
			glm::vec3 v2 = sphericalToCartesian(radius, theta1, phi2);
			glm::vec3 v3 = sphericalToCartesian(radius, theta2, phi2);
			glm::vec3 v4 = sphericalToCartesian(radius, theta2, phi1);

			// Vertex UVs
			glm::vec2 uv1 =
			{
				(float)(widthSegments - j) / (float)widthSegments,
				(float)(heightSegments - i) / (float)heightSegments
				
			};

			glm::vec2 uv2 =
			{
				(float)(widthSegments - j - 1) / (float)widthSegments,
				(float)(heightSegments - i) / (float)heightSegments
				
			};

			glm::vec2 uv3 =
			{
				(float)(widthSegments - j - 1) / (float)widthSegments,
				(float)(heightSegments - i - 1) / (float)heightSegments
				
			};

			glm::vec2 uv4 =
			{
				(float)(widthSegments - j) / (float)widthSegments,
				(float)(heightSegments - i - 1) / (float)heightSegments
			};

			// Vertex normals
			glm::vec3 n1 = glm::normalize(v1);
			glm::vec3 n2 = glm::normalize(v2);
			glm::vec3 n3 = glm::normalize(v3);
			glm::vec3 n4 = glm::normalize(v4);

			// Vertices
			Vertex vert1 = { v1, n1, uv1 };
			Vertex vert2 = { v2, n2, uv2 };
			Vertex vert3 = { v3, n3, uv3 };
			Vertex vert4 = { v4, n4, uv4 };

			// Top cap
			if (i == 0)
			{
				newSphere.appendTriangle(vert1, vert3, vert4);
			}
			// Bottom cap
			else if (i + 1 == heightSegments)
			{
				newSphere.appendTriangle(vert3, vert1, vert2);
			}
			// Everything else
			else
			{
				newSphere.appendTriangle(vert1, vert2, vert4);
				newSphere.appendTriangle(vert2, vert3, vert4);
			}
		}
	}
	newSphere.makeIndicesSmooth();
	newSphere.makeTangents();
	return newSphere;
}

glm::vec3 Sphere::sphericalToCartesian(const float radius, const float theta, const float phi)
{
	// Standard equation assumes z is up so need to swap z and y
	return glm::vec3(
		radius * std::sin(theta) * std::cos(phi),
		radius * std::cos(theta),
		radius * std::sin(theta) * std::sin(phi));
}