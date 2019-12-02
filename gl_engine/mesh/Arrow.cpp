#include <cassert>

#include "Arrow.h"
#include "Vertex.h"

Mesh Arrow::createArrow()
{
	Mesh newArrow;
	
	// Up face
	newArrow += createArrowFace(
		glm::vec3(+0.0f, +2.0f, +3.0f),
		glm::vec3(-2.0f, +2.0f, +1.0f),
		glm::vec3(+2.0f, +2.0f, +1.0f),
		glm::vec3(-1.0f, +2.0f, +1.0f),
		glm::vec3(+1.0f, +2.0f, +1.0f),
		glm::vec3(-1.0f, +2.0f, -1.0f),
		glm::vec3(+1.0f, +2.0f, -1.0f));

	// Down face
	newArrow += createArrowFace(
		glm::vec3(+0.0f, +0.0f, +3.0f),
		glm::vec3(+2.0f, +0.0f, +1.0f),
		glm::vec3(-2.0f, +0.0f, +1.0f),
		glm::vec3(+1.0f, +0.0f, +1.0f),
		glm::vec3(-1.0f, +0.0f, +1.0f),
		glm::vec3(+1.0f, +0.0f, -1.0f),
		glm::vec3(-1.0f, +0.0f, -1.0f));

	// Back face 1
	newArrow += createRectangleFace(
		glm::vec3(-1.0, +2.0, -1.0),
		glm::vec3(+1.0, +2.0, -1.0),
		glm::vec3(-1.0, +0.0, -1.0),
		glm::vec3(+1.0, +0.0, -1.0));

	// Back face 2
	newArrow += createRectangleFace(
		glm::vec3(-2.0, +2.0, +1.0),
		glm::vec3(-1.0, +2.0, +1.0),
		glm::vec3(-2.0, +0.0, +1.0),
		glm::vec3(-1.0, +0.0, +1.0));

	// Back face 3
	newArrow += createRectangleFace(
		glm::vec3(+1.0, +2.0, +1.0),
		glm::vec3(+2.0, +2.0, +1.0),
		glm::vec3(+1.0, +0.0, +1.0),
		glm::vec3(+2.0, +0.0, +1.0));

	// Left face
	newArrow += createRectangleFace(
		glm::vec3(-1.0, +2.0, +1.0),
		glm::vec3(-1.0, +2.0, -1.0),
		glm::vec3(-1.0, +0.0, +1.0),
		glm::vec3(-1.0, +0.0, -1.0));

	// Right face
	newArrow += createRectangleFace(
		glm::vec3(+1.0, +2.0, -1.0),
		glm::vec3(+1.0, +2.0, +1.0),
		glm::vec3(+1.0, +0.0, -1.0),
		glm::vec3(+1.0, +0.0, +1.0));

	// Left diagonal face
	newArrow += createRectangleFace(
		glm::vec3(-0.0, +2.0, +3.0),
		glm::vec3(-2.0, +2.0, +1.0),
		glm::vec3(-0.0, +0.0, +3.0),
		glm::vec3(-2.0, +0.0, +1.0));

	// Right diagonal face
	newArrow += createRectangleFace(
		glm::vec3(+2.0, +2.0, +1.0),
		glm::vec3(-0.0, +2.0, +3.0),
		glm::vec3(+2.0, +0.0, +1.0),
		glm::vec3(-0.0, +0.0, +3.0));


	newArrow.makeIndicesSmooth();
	newArrow.makeTangents();

	return newArrow;
}
Mesh Arrow::createRectangleFace(glm::vec3 tl, glm::vec3 tr, glm::vec3 bl, glm::vec3 br)
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
	Vertex vert_tl = { tl, faceNormal, uv2 };
	Vertex vert_tr = { tr, faceNormal, uv1 };
	Vertex vert_bl = { bl, faceNormal, uv4 };
	Vertex vert_br = { br, faceNormal, uv3 };

	newFace.appendTriangle(vert_tl, vert_tr, vert_bl);
	newFace.appendTriangle(vert_tr, vert_br, vert_bl);

	return newFace;
}
Mesh Arrow::createArrowFace(glm::vec3 t, glm::vec3 cl, glm::vec3 cr, glm::vec3 il, glm::vec3 ir, glm::vec3 bl, glm::vec3 br)
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

	Mesh newFace;

	// Vertex Normals
	glm::vec3 faceNormal = glm::cross(ir - il, bl - il);

	// VertexUVs
	glm::vec2 uv_t  = { 0.5f,  1.0f };
	glm::vec2 uv_cr = { 0.0f,  0.5f };
	glm::vec2 uv_cl = { 1.0f,  0.5f };
	glm::vec2 uv_ir = { 0.25f, 0.5f };
	glm::vec2 uv_il = { 0.75f, 0.5f };
	glm::vec2 uv_br = { 0.25f, 0.0f };
	glm::vec2 uv_bl = { 0.75f, 0.0f };

	// Vertices
	Vertex vert_t  = { t,  faceNormal, uv_t  };
	Vertex vert_cl = { cl, faceNormal, uv_cl };
	Vertex vert_cr = { cr, faceNormal, uv_cr };
	Vertex vert_il = { il, faceNormal, uv_il };
	Vertex vert_ir = { ir, faceNormal, uv_ir };
	Vertex vert_bl = { bl, faceNormal, uv_bl };
	Vertex vert_br = { br, faceNormal, uv_br };

	newFace.appendTriangle(vert_t, vert_cr, vert_cl);
	newFace.appendTriangle(vert_il, vert_ir, vert_bl);
	newFace.appendTriangle(vert_ir, vert_br, vert_bl);

	return newFace;
}