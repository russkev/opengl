#define _USE_MATH_DEFINES

#include <cmath>

#include "Sphere.h"
#include "../Vertex.h"

ShapeData Sphere::createSphere(float radius, uint32_t widthSegments, uint32_t heightSegments)
{
	ShapeData newSphere;
	// Thanks to user bobobobo on stackexchange
	// Source found here: 
	// https://gamedev.stackexchange.com/questions/16585/how-do-you-programmatically-generate-a-sphere
	//
	for (uint32_t i = 0; i < 1/*heightSegments*/; i++)
	{
		float theta1 = ((float)(i)	   / (float)heightSegments) * 2 * (float)M_PI;
		float theta2 = ((float)(i + 1) / (float)heightSegments) * 2 * (float)M_PI;

		for (uint32_t j = 0; j < widthSegments; ++j)
		{
			float phi1 = ((float)(j)	 / (float)widthSegments) * 2 * (float)M_PI;
			float phi2 = ((float)(j + 1) / (float)widthSegments) * 2 * (float)M_PI;

			// Vertex positions
			glm::vec3 v1 =
			{
				radius * std::sin(phi1) * std::cos(theta1),
				radius * std::sin(phi1) * std::sin(theta1),
				radius * std::cos(phi1)
			};

			glm::vec3 v2 = 
			{
				radius * std::sin(phi2) * std::cos(theta1),
				radius * std::sin(phi2) * std::sin(theta1),
				radius * std::cos(phi2)
			};

			glm::vec3 v3 =
			{
				radius * std::sin(phi2) * std::cos(theta2),
				radius * std::sin(phi2) * std::sin(theta2),
				radius * std::cos(phi2)
			};

			glm::vec3 v4 =
			{
				radius * std::sin(phi1) * std::cos(theta2),
				radius * std::sin(phi1) * std::sin(theta2),
				radius * std::cos(phi1)
			};

			// Vertex UVs
			glm::vec2 uv1 =
			{
				(float)(i) / (float)heightSegments,
				(float)(j) / (float)widthSegments
			};

			glm::vec2 uv2 =
			{
				(float)(i) / (float)heightSegments,
				(float)(j + 1) / (float)widthSegments
			};

			glm::vec2 uv3 =
			{
				(float)(i + 1) / (float)heightSegments,
				(float)(j + 1) / (float)widthSegments
			};

			glm::vec2 uv4 =
			{
				(float)(i + 1) / (float)heightSegments,
				(float)(j) / (float)widthSegments
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
			if (j == 0)
			{
				newSphere.appendTriangle(vert1, vert3, vert4);
			}
			// Bottom cap
			else if (j + 1 == heightSegments)
			{
				newSphere.appendTriangle(vert3, vert1, vert2);
			}
			// Everything else
			else
			{
				newSphere.appendTriangle(vert1, vert2, vert4);
				//newSphere.appendTriangle(vert2, vert3, vert4);
			}
		}
	}
	newSphere.makeIndicesFaceted();
	return newSphere;
}