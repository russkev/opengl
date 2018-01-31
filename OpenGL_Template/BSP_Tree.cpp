#include "BSP_Tree.h"

void BSP_Tree::BuildBSPTree(BSP_Tree *s_tree, const std::vector<ShapeData>& s_polygons)
{
	ShapeData			*root;
	ShapeGenerator		ShapeGen;

	root					= s_polygons.data;
	//s_tree->m_partition		= std::move(s_polygons.at(0));
	std::copy(s_polygons.begin(), s_polygons.begin + 1, s_tree->m_partition);
}