#include "WindowQuad.h"

#include <glm/glm.hpp>

namespace gl_engine_exe
{
	Mesh WindowQuad::create_windowQuad()
	{
		Mesh windowQuad;

		// Vertix order is like this:
		//
		//			+1.0
		//			  |
		//		  v1-----v2
		// -1.0 ___|__|__|___ +1.0
		//		   |  |  |
		//		  v3-----v4
		//			  |
		//			-1.0
		//

		//// Vertex positions
		//glm::vec3 v1 { -1.0f, +1.0f, +0.0f };
		//glm::vec3 v2 { -1.0f, -1.0f, +0.0f };
		//glm::vec3 v3 { +1.0f, +1.0f, +0.0f };
		//glm::vec3 v4 { +1.0f, -1.0f, +0.0f };

		//glm::vec3 normal{ 0.0f, 1.0f, 0.0f };

		//// Vertex uvs
		//glm::vec2 uv1{ 0.0f, 1.0f };
		//glm::vec2 uv2{ 0.0f, 0.0f };
		//glm::vec2 uv3{ 1.0f, 1.0f };
		//glm::vec2 uv4{ 1.0f, 0.0f };


		// Vertex positions
		glm::vec3 v1{ -1.0f, +1.0f, +0.0f };
		glm::vec3 v2{ +1.0f, +1.0f, +0.0f };
		glm::vec3 v3{ -1.0f, -1.0f, +0.0f };
		glm::vec3 v4{ +1.0f, -1.0f, +0.0f };

		glm::vec3 normal{ 0.0f, 1.0f, 0.0f };

		// Vertex uvs
		glm::vec2 uv1{ 0.0f, 1.0f };
		glm::vec2 uv2{ 1.0f, 1.0f };
		glm::vec2 uv3{ 0.0f, 0.0f };
		glm::vec2 uv4{ 1.0f, 0.0f };

		Vertex vert1{ v1, normal, uv1 };
		Vertex vert2{ v2, normal, uv2 };
		Vertex vert3{ v3, normal, uv3 };
		Vertex vert4{ v4, normal, uv4 };

		windowQuad.append_triangle(vert1, vert3, vert2);
		windowQuad.append_triangle(vert2, vert3, vert4);

		windowQuad.make_indices_smooth();
		windowQuad.make_tangents();


		return windowQuad;
	}
} // namespace gl_engine