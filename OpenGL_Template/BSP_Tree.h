#pragma once
#include "ShapeGenerator.h"
#include "ShapeData.h"
#include <vector>

struct BSP_Tree
{
public:
	void BuildBSPTree(BSP_Tree *s_tree, const std::vector<ShapeData>& s_polygons);

private:
	ShapeData				m_partition;
	std::vector<ShapeData>	m_polygons;
	BSP_Tree				*m_front;
	BSP_Tree				*m_back;
};
