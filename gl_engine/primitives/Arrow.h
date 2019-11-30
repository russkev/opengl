#pragma once

#include "../ShapeData.h"

struct Arrow
{
	static ShapeData createArrow();
private:
	static ShapeData createRectangleFace(glm::vec3 tl, glm::vec3 tr, glm::vec3 bl, glm::vec3 br);
	static ShapeData createArrowFace(glm::vec3 t, glm::vec3 cl, glm::vec3 cr, glm::vec3 il, glm::vec3 ir, glm::vec3 bl, glm::vec3 br);

};