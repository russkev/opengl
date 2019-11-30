#pragma once
#include <vector>

#include "ShapeData.h"
#include "Utilities.h"

struct VBO_Indexer
{
	static std::vector<unsigned short> index(ShapeData vertices);
};
