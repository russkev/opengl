#include <cassert>

#include "Cube.h"
#include "Vertex.h"

namespace gl_engine
{
	Mesh Cube::createCube()
	{
		return createCube(1.0f);
	}

	Mesh Cube::createCube(const float width)
	{
		float halfWidth = width / 2.0f;
		Mesh newCube;

		// Up Face
		newCube += createCubeFace(
			glm::vec3(-halfWidth, width, +halfWidth),
			glm::vec3(+halfWidth, width, +halfWidth),
			glm::vec3(-halfWidth, width, -halfWidth),
			glm::vec3(+halfWidth, width, -halfWidth)
		);

		// Bottom Face
		newCube += createCubeFace(
			glm::vec3(+halfWidth, 0, +halfWidth),
			glm::vec3(-halfWidth, 0, +halfWidth),
			glm::vec3(+halfWidth, 0, -halfWidth),
			glm::vec3(-halfWidth, 0, -halfWidth)
		);

		// Right Face
		newCube += createCubeFace(
			glm::vec3(+halfWidth, width, -halfWidth),
			glm::vec3(+halfWidth, width, +halfWidth),
			glm::vec3(+halfWidth, 0, -halfWidth),
			glm::vec3(+halfWidth, 0, +halfWidth)
		);

		// Left Face
		newCube += createCubeFace(
			glm::vec3(-halfWidth, width, +halfWidth),
			glm::vec3(-halfWidth, width, -halfWidth),
			glm::vec3(-halfWidth, 0, +halfWidth),
			glm::vec3(-halfWidth, 0, -halfWidth)
		);

		// Front Face
		newCube += createCubeFace(
			glm::vec3(-halfWidth, width, -halfWidth),
			glm::vec3(+halfWidth, width, -halfWidth),
			glm::vec3(-halfWidth, 0, -halfWidth),
			glm::vec3(+halfWidth, 0, -halfWidth)
		);

		// Back Face
		newCube += createCubeFace(
			glm::vec3(+halfWidth, width, +halfWidth),
			glm::vec3(-halfWidth, width, +halfWidth),
			glm::vec3(+halfWidth, 0, +halfWidth),
			glm::vec3(-halfWidth, 0, +halfWidth)
		);

		newCube.makeIndicesSmooth();
		newCube.makeTangents();

		return newCube;
	}

	Mesh Cube::createCubeFace(glm::vec3 tl, glm::vec3 tr, glm::vec3 bl, glm::vec3 br)
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
		glm::vec3 faceNormal = glm::cross(tr - tl, bl - tl);

		// Vertex UVs
		glm::vec2 uv1 = { 0.0f, 1.0f };
		glm::vec2 uv2 = { 1.0f, 1.0f };
		glm::vec2 uv3 = { 0.0f, 0.0f };
		glm::vec2 uv4 = { 1.0f, 0.0f };

		// Vertices
		Vertex vert1 = { tl, faceNormal, uv2 };
		Vertex vert2 = { tr, faceNormal, uv1 };
		Vertex vert3 = { bl, faceNormal, uv4 };
		Vertex vert4 = { br, faceNormal, uv3 };

		newFace.appendTriangle(vert1, vert2, vert3);
		newFace.appendTriangle(vert2, vert4, vert3);

		return newFace;
	}
} // namespace gl_engine


