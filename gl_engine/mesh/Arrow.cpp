#include "pch.h"
#include "Arrow.h"

#include "Vertex.h"

namespace glen
{
	// // ----- MAIN ----- // //
	Mesh Arrow::create_arrow()
	{
		return create_arrow(1.0f);
	}

	Mesh Arrow::create_arrow(const float length)
	{
		Mesh new_arrow;

		Mesh up_face = create_arrow_face(
			glm::vec3(+0.0f, +2.0f, +3.0f),
			glm::vec3(-2.0f, +2.0f, +1.0f),
			glm::vec3(+2.0f, +2.0f, +1.0f),
			glm::vec3(-1.0f, +2.0f, +1.0f),
			glm::vec3(+1.0f, +2.0f, +1.0f),
			glm::vec3(-1.0f, +2.0f, -length),
			glm::vec3(+1.0f, +2.0f, -length));

		Mesh down_face = create_arrow_face(
			glm::vec3(+0.0f, +0.0f, +3.0f),
			glm::vec3(+2.0f, +0.0f, +1.0f),
			glm::vec3(-2.0f, +0.0f, +1.0f),
			glm::vec3(+1.0f, +0.0f, +1.0f),
			glm::vec3(-1.0f, +0.0f, +1.0f),
			glm::vec3(+1.0f, +0.0f, -length),
			glm::vec3(-1.0f, +0.0f, -length));

		Mesh back_face_1 = create_rectangle_face(
			glm::vec3(-1.0, +2.0, -length),
			glm::vec3(+1.0, +2.0, -length),
			glm::vec3(-1.0, +0.0, -length),
			glm::vec3(+1.0, +0.0, -length));

		Mesh back_face_2 = create_rectangle_face(
			glm::vec3(-2.0, +2.0, +1.0),
			glm::vec3(-1.0, +2.0, +1.0),
			glm::vec3(-2.0, +0.0, +1.0),
			glm::vec3(-1.0, +0.0, +1.0));

		Mesh back_face_3 = create_rectangle_face(
			glm::vec3(+1.0, +2.0, +1.0),
			glm::vec3(+2.0, +2.0, +1.0),
			glm::vec3(+1.0, +0.0, +1.0),
			glm::vec3(+2.0, +0.0, +1.0));

		Mesh left_face = create_rectangle_face(
			glm::vec3(-1.0, +2.0, +1.0),
			glm::vec3(-1.0, +2.0, -length),
			glm::vec3(-1.0, +0.0, +1.0),
			glm::vec3(-1.0, +0.0, -length));

		Mesh right_face = create_rectangle_face(
			glm::vec3(+1.0, +2.0, -length),
			glm::vec3(+1.0, +2.0, +1.0),
			glm::vec3(+1.0, +0.0, -length),
			glm::vec3(+1.0, +0.0, +1.0));

		Mesh left_diagonal_face = create_rectangle_face(
			glm::vec3(-0.0, +2.0, +3.0),
			glm::vec3(-2.0, +2.0, +1.0),
			glm::vec3(-0.0, +0.0, +3.0),
			glm::vec3(-2.0, +0.0, +1.0));

		Mesh right_diagonal_face = create_rectangle_face(
			glm::vec3(+2.0, +2.0, +1.0),
			glm::vec3(-0.0, +2.0, +3.0),
			glm::vec3(+2.0, +0.0, +1.0),
			glm::vec3(-0.0, +0.0, +3.0));

		new_arrow += up_face;
		new_arrow += down_face;
		new_arrow += back_face_1;
		new_arrow += back_face_2;
		new_arrow += back_face_3;
		new_arrow += left_face;
		new_arrow += right_face;
		new_arrow += left_diagonal_face;
		new_arrow += right_diagonal_face;

		new_arrow.make_indices_smooth();
		new_arrow.make_tangents();

		return new_arrow;
	}

	// // ----- HELPER ----- // //
	Mesh Arrow::create_rectangle_face(glm::vec3 tl, glm::vec3 tr, glm::vec3 bl, glm::vec3 br)
	{
		// Vertix order is like this:
		//
		//		  tl-----tr
		//		   |     |
		//		   |     |
		//		  bl-----br
		//
		Mesh new_face;

		// Vertex Normals
		glm::vec3 face_normal = glm::cross(tr - tl, bl - tl);

		// Vertex UVs
		glm::vec2 uv1 = { 0.0f, 1.0f };
		glm::vec2 uv2 = { 1.0f, 1.0f };
		glm::vec2 uv3 = { 0.0f, 0.0f };
		glm::vec2 uv4 = { 1.0f, 0.0f };

		// Vertices
		Vertex vert_tl = { tl, face_normal, uv2 };
		Vertex vert_tr = { tr, face_normal, uv1 };
		Vertex vert_bl = { bl, face_normal, uv4 };
		Vertex vert_br = { br, face_normal, uv3 };

		new_face.append_triangle(vert_tl, vert_tr, vert_bl);
		new_face.append_triangle(vert_tr, vert_br, vert_bl);

		return new_face;
	}
	Mesh Arrow::create_arrow_face(glm::vec3 t, glm::vec3 cl, glm::vec3 cr, glm::vec3 il, glm::vec3 ir, glm::vec3 bl, glm::vec3 br)
	{
		// Vertix order is like this:
		//            t
		//           / \
		//         /     \
		//       /         \
		//     /             \ 
		//   cl---il     ir---cr
		//		   |     |
		//		   |     |
		//		  bl-----br
		//

		Mesh new_face;

		// Vertex Normals
		glm::vec3 face_normal = glm::cross(ir - il, bl - il);

		// VertexUVs
		glm::vec2 uv_t = { 0.5f,  1.0f };
		glm::vec2 uv_cr = { 0.0f,  0.5f };
		glm::vec2 uv_cl = { 1.0f,  0.5f };
		glm::vec2 uv_ir = { 0.25f, 0.5f };
		glm::vec2 uv_il = { 0.75f, 0.5f };
		glm::vec2 uv_br = { 0.25f, 0.0f };
		glm::vec2 uv_bl = { 0.75f, 0.0f };

		// Vertices
		Vertex vert_t = { t,  face_normal, uv_t };
		Vertex vert_cl = { cl, face_normal, uv_cl };
		Vertex vert_cr = { cr, face_normal, uv_cr };
		Vertex vert_il = { il, face_normal, uv_il };
		Vertex vert_ir = { ir, face_normal, uv_ir };
		Vertex vert_bl = { bl, face_normal, uv_bl };
		Vertex vert_br = { br, face_normal, uv_br };

		new_face.append_triangle(vert_t, vert_cr, vert_cl);
		new_face.append_triangle(vert_il, vert_ir, vert_bl);
		new_face.append_triangle(vert_ir, vert_br, vert_bl);

		return new_face;
	}
}
