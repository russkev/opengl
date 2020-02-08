#include <cassert>

#include "Cube.h"
#include "Vertex.h"

namespace gl_engine_exe
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

		// Up Face
		newCube += create_cube_face(
			glm::vec3(-halfWidth, width, +halfWidth),
			glm::vec3(+halfWidth, width, +halfWidth),
			glm::vec3(-halfWidth, width, -halfWidth),
			glm::vec3(+halfWidth, width, -halfWidth)
		);

		// Bottom Face
		newCube += create_cube_face(
			glm::vec3(+halfWidth, 0, +halfWidth),
			glm::vec3(-halfWidth, 0, +halfWidth),
			glm::vec3(+halfWidth, 0, -halfWidth),
			glm::vec3(-halfWidth, 0, -halfWidth)
		);

		// Right Face
		newCube += create_cube_face(
			glm::vec3(+halfWidth, width, -halfWidth),
			glm::vec3(+halfWidth, width, +halfWidth),
			glm::vec3(+halfWidth, 0, -halfWidth),
			glm::vec3(+halfWidth, 0, +halfWidth)
		);

		// Left Face
		newCube += create_cube_face(
			glm::vec3(-halfWidth, width, +halfWidth),
			glm::vec3(-halfWidth, width, -halfWidth),
			glm::vec3(-halfWidth, 0, +halfWidth),
			glm::vec3(-halfWidth, 0, -halfWidth)
		);

		// Front Face
		newCube += create_cube_face(
			glm::vec3(-halfWidth, width, -halfWidth),
			glm::vec3(+halfWidth, width, -halfWidth),
			glm::vec3(-halfWidth, 0, -halfWidth),
			glm::vec3(+halfWidth, 0, -halfWidth)
		);

		// Back Face
		newCube += create_cube_face(
			glm::vec3(+halfWidth, width, +halfWidth),
			glm::vec3(-halfWidth, width, +halfWidth),
			glm::vec3(+halfWidth, 0, +halfWidth),
			glm::vec3(-halfWidth, 0, +halfWidth)
		);

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
		glm::vec3 faceNormal = glm::cross(top_right - top_left, bottom_left - top_left);

		// Vertex UVs
		glm::vec2 uv1 = { 0.0f, 1.0f };
		glm::vec2 uv2 = { 1.0f, 1.0f };
		glm::vec2 uv3 = { 0.0f, 0.0f };
		glm::vec2 uv4 = { 1.0f, 0.0f };

		// Vertices
		Vertex vert1 = { top_left, faceNormal, uv2 };
		Vertex vert2 = { top_right, faceNormal, uv1 };
		Vertex vert3 = { bottom_left, faceNormal, uv4 };
		Vertex vert4 = { bottom_right, faceNormal, uv3 };

		newFace.append_triangle(vert1, vert2, vert3);
		newFace.append_triangle(vert2, vert4, vert3);

		return newFace;
	}
} // namespace gl_engine


