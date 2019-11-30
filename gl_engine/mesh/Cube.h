#pragma once

#include "Mesh.h"

struct Cube
{
	static Mesh createCube();
	static Mesh createCube(const float width);
private:
	static Mesh createCubeFace(glm::vec3 tl, glm::vec3 tr, glm::vec3 bl, glm::vec3 br);
};