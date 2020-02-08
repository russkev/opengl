#include "pch.h"

#include "Cube.h"
#include "Vertex.h"

namespace glen
{
	// // ----- MAIN ----- // //
	Mesh Cube::create_cube()
	{
		return create_cube(1.0f);
	}

	Mesh Cube::create_cube(const float width)
	{
		float halfWidth = width / 2.0f;
		Mesh newCube;


		Mesh up_face = create_cube_face(
			glm::vec3(-halfWidth, width, +halfWidth),
			glm::vec3(+halfWidth, width, +halfWidth),
			glm::vec3(-halfWidth, width, -halfWidth),
			glm::vec3(+halfWidth, width, -halfWidth)
		);

		Mesh bottom_face = create_cube_face(
			glm::vec3(+halfWidth, 0, +halfWidth),
			glm::vec3(-halfWidth, 0, +halfWidth),
			glm::vec3(+halfWidth, 0, -halfWidth),
			glm::vec3(-halfWidth, 0, -halfWidth)
		);
		newCube += bottom_face;

		Mesh right_face = create_cube_face(
			glm::vec3(+halfWidth, width, -halfWidth),
			glm::vec3(+halfWidth, width, +halfWidth),
			glm::vec3(+halfWidth, 0, -halfWidth),
			glm::vec3(+halfWidth, 0, +halfWidth)
		);
		newCube += right_face;

		Mesh left_face = create_cube_face(
			glm::vec3(-halfWidth, width, +halfWidth),
			glm::vec3(-halfWidth, width, -halfWidth),
			glm::vec3(-halfWidth, 0, +halfWidth),
			glm::vec3(-halfWidth, 0, -halfWidth)
		);
		newCube += left_face;

		Mesh front_face = create_cube_face(
			glm::vec3(-halfWidth, width, -halfWidth),
			glm::vec3(+halfWidth, width, -halfWidth),
			glm::vec3(-halfWidth, 0, -halfWidth),
			glm::vec3(+halfWidth, 0, -halfWidth)
		);
		newCube += front_face;

		Mesh back_face = create_cube_face(
			glm::vec3(+halfWidth, width, +halfWidth),
			glm::vec3(-halfWidth, width, +halfWidth),
			glm::vec3(+halfWidth, 0, +halfWidth),
			glm::vec3(-halfWidth, 0, +halfWidth)
		);
		newCube += back_face;

		newCube += up_face;
		newCube += bottom_face;
		newCube += right_face;
		newCube += left_face;
		newCube += front_face;
		newCube += back_face;

		newCube.make_indices_smooth();
		newCube.make_tangents();

		return newCube;
	}

	// // ----- HELPER ----- // //
	Mesh Cube::create_cube_face(glm::vec3 top_left, glm::vec3 top_right, glm::vec3 bottom_left, glm::vec3 bottom_right)
	{
		// Vertix order is like this:
		//
		//		  tl-----tr
		//		   |     |
		//		   |     |
		//		  bl-----br
		//
		Mesh newFace;

		// Vertex Normals
		glm::vec3 face_normal = glm::cross(top_right - top_left, bottom_left - top_left);

		// Vertex UVs
		glm::vec2 uv1 = { 0.0f, 1.0f };
		glm::vec2 uv2 = { 1.0f, 1.0f };
		glm::vec2 uv3 = { 0.0f, 0.0f };
		glm::vec2 uv4 = { 1.0f, 0.0f };

		// Vertices
		Vertex vert1 = { top_left, face_normal, uv2 };
		Vertex vert2 = { top_right, face_normal, uv1 };
		Vertex vert3 = { bottom_left, face_normal, uv4 };
		Vertex vert4 = { bottom_right, face_normal, uv3 };

		newFace.append_triangle(vert1, vert2, vert3);
		newFace.append_triangle(vert2, vert4, vert3);

		return newFace;
	}
}