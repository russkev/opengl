#pragma once

#include "../ShapeData.h"

struct Cube
{
	static ShapeData createCube();
	static ShapeData createCube(const float width);
private:
	static ShapeData createCubeFace(glm::vec3 tl, glm::vec3 tr, glm::vec3 bl, glm::vec3 br);
};