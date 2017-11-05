#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "ShapeData.h"

struct ShapeHolder
{
	// // ----- Member Variables ----- // //
	std::vector<ShapeData>	m_shapes;
	std::vector<glm::mat4>	m_transforms;
	std::vector<glm::ivec2> m_connections;
};