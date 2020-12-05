#include "pch.h"

#include "Plane.h"
#include "Vertex.h"

namespace glen
{
	// // ------ MAIN ----- // //
	Mesh Plane::create_plane()
	{
		return create_plane(1.0f, 1.0f, 1, 1);
	}

	Mesh Plane::create_plane(const float width, const float height)
	{
		return create_plane(width, height, 1, 1);
	}

	Mesh Plane::create_plane(const float width, const float height, const uint16_t width_segments, const uint16_t height_segments)
	{
		assert(width > 0);
		assert(height > 0);
		assert(width_segments > 0);
		assert(height_segments > 0);

		Mesh new_plane;
		float width_offset = width / (float)width_segments;
		float height_offset = height / (float)height_segments;
		float width_uv_offset = 1 / (float)width_segments;
		float height_uv_offset = 1 / (float)height_segments;
		float start_x = width * -0.5f;
		float start_z = height * -0.5f;
		glm::vec3 normal = { 0.0, 1.0, 0.0 };

		for (uint16_t x = 0; x < width_segments; ++x)
		{
			for (uint16_t z = 0; z < height_segments; ++z)
			{
				// Vertix order is like this:
				//
				//		  v1-----v2
				//		   |     |
				//		   |     |
				//		  v3-----v4
				//

				// Vertex positions
				glm::vec3 v1 = { start_x + width_offset * (float)(x),		0.0f,	start_z + height_offset * (float)(z) };
				glm::vec3 v2 = { start_x + width_offset * (float)(x + 1),	0.0f,	start_z + height_offset * (float)(z) };
				glm::vec3 v3 = { start_x + width_offset * (float)(x),		0.0f,	start_z + height_offset * (float)(z + 1) };
				glm::vec3 v4 = { start_x + width_offset * (float)(x + 1),	0.0f,	start_z + height_offset * (float)(z + 1) };

				// Vertex UVs
				glm::vec2 uv1 = { width_uv_offset * (float)(x + 1),	height_uv_offset * (float)(height_segments - z) };
				glm::vec2 uv2 = { width_uv_offset * (float)(x),		height_uv_offset * (float)(height_segments - z) };
				glm::vec2 uv3 = { width_uv_offset * (float)(x + 1),	height_uv_offset * (float)(height_segments - z - 1) };
				glm::vec2 uv4 = { width_uv_offset * (float)(x),		height_uv_offset * (float)(height_segments - z - 1) };

				// Vertices
				Vertex vert1 = { v1, normal, uv1 };
				Vertex vert2 = { v2, normal, uv2 };
				Vertex vert3 = { v3, normal, uv3 };
				Vertex vert4 = { v4, normal, uv4 };

				new_plane.append_triangle(vert1, vert3, vert2);
				new_plane.append_triangle(vert3, vert4, vert2);
			}
		}
		new_plane.make_indices_smooth();
		new_plane.make_tangents();
		return new_plane;
	}
}