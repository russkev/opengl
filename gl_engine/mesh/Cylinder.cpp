#define _USE_MATH_DEFINES

#include <cmath>
#include <cassert>

#include "Cylinder.h"
#include "Plane.h"

namespace gl_engine
{
	Mesh Cylinder::create_cylinder()
	{
		return create_cylinder(0.5f, 2.0f);
	}
	Mesh Cylinder::create_cylinder(const float radius, const float height)
	{
		return create_cylinder(radius, height, 24, 1);
	}

	Mesh Cylinder::create_cylinder(const float radius, const float height, const uint16_t width_segments, const uint16_t height_segments)
	{
		assert(radius > 0);
		assert(height > 0);
		assert(width_segments > 2);
		assert(height_segments > 0);

		Mesh newCylinder;
		float heightOffset = height / (float)height_segments;
		float widthUVOffset = 1 / (float)width_segments;
		float heightUVOffset = 1 / (float)height_segments;

		for (uint16_t i = 0; i < height_segments; ++i)
		{
			for (uint16_t j = 0; j < width_segments; ++j)
			{
				// Vertix order is like this:
				//
				//		  v3-----v4
				//		   |     |
				//		   |     |
				//		  v1-----v2
				//

				float theta1 = ((float)(j) / (float)width_segments) * 2 * (float)M_PI;
				float theta2 = ((float)(j + 1) / (float)width_segments) * 2 * (float)M_PI;

				// Vertex positions
				glm::vec3 v1 = radial_to_cartesian(radius, theta1, heightOffset * (i));
				glm::vec3 v2 = radial_to_cartesian(radius, theta2, heightOffset * (i));
				glm::vec3 v3 = radial_to_cartesian(radius, theta1, heightOffset * (i + 1));
				glm::vec3 v4 = radial_to_cartesian(radius, theta2, heightOffset * (i + 1));

				// Vertex normals
				glm::vec3 n_theta1 = radial_to_cartesian(1, theta1, 0);
				glm::vec3 n_theta2 = radial_to_cartesian(1, theta2, 0);

				// Vertex UVs
				glm::vec2 uv1 = { (width_segments - j)     * widthUVOffset, (i)* heightUVOffset };
				glm::vec2 uv2 = { (width_segments - j - 1) * widthUVOffset, (i)* heightUVOffset };
				glm::vec2 uv3 = { (width_segments - j)     * widthUVOffset, (i + 1) * heightUVOffset };
				glm::vec2 uv4 = { (width_segments - j - 1) * widthUVOffset, (i + 1) * heightUVOffset };

				// Vertices
				Vertex vert1 = { v1, n_theta1, uv1 };
				Vertex vert2 = { v2, n_theta2, uv2 };
				Vertex vert3 = { v3, n_theta1, uv3 };
				Vertex vert4 = { v4, n_theta2, uv4 };

				newCylinder.append_triangle(vert1, vert4, vert2);
				newCylinder.append_triangle(vert1, vert3, vert4);

				// Bottom cap
				if (i == 0)
				{
					// Vertex positions
					glm::vec3 v_centre = { 0.0f, 0.0f, 0.0f };

					// Vertex normals
					glm::vec3 n_cap = { 0.0f, -1.0f, 0.0f };

					// Vertex UVs
					glm::vec2 uv_cap1 = { (std::cos(theta1) / 2) + 0.5, (std::sin(theta1) / 2) + 0.5 };
					glm::vec2 uv_cap2 = { (std::cos(theta2) / 2) + 0.5, (std::sin(theta2) / 2) + 0.5 };
					glm::vec2 uv_cap3 = { 0.5f, 0.5f };

					// Vertices
					Vertex vert_cap1 = { v1, n_cap, uv_cap1 };
					Vertex vert_cap2 = { v2, n_cap, uv_cap2 };
					Vertex vert_cap3 = { v_centre, n_cap, uv_cap3 };

					newCylinder.append_triangle(vert_cap1, vert_cap2, vert_cap3);
				}

				// Top cap
				if (i == height_segments - 1)
				{
					// Vertex positions
					glm::vec3 v_centre = { 0.0f, height, 0.0f };

					// Vertex normals
					glm::vec3 n_cap = { 0.0f, 1.0f, 0.0f };

					// Vertex UVs
					glm::vec2 uv_cap1 = { (std::cos(theta1) / 2) + 0.5, (-std::sin(theta1) / 2) + 0.5 };
					glm::vec2 uv_cap2 = { (std::cos(theta2) / 2) + 0.5, (-std::sin(theta2) / 2) + 0.5 };
					glm::vec2 uv_cap3 = { 0.5f, 0.5f };

					// Vertices
					Vertex vert_cap1 = { v4, n_cap, uv_cap2 };
					Vertex vert_cap2 = { v3, n_cap, uv_cap1 };
					Vertex vert_cap3 = { v_centre, n_cap, uv_cap3 };

					newCylinder.append_triangle(vert_cap1, vert_cap2, vert_cap3);
				}
			}
		}
		newCylinder.make_indices_smooth();
		newCylinder.make_tangents();
		return newCylinder;
	}

	glm::vec3 Cylinder::radial_to_cartesian(float radius, float theta, float height)
	{
		return glm::vec3(radius * std::cos(theta), height, radius * std::sin(theta));
	}
} // namespace gl_engine