#pragma once
#include "ShapeData.h"
#include "VectorUtils.h"

//struct VertexIsSimilar
//{
//	static constexpr float DISTANCE_THRESHOLD = 0.01f;
//	bool compare (const ShapeData::vertexType* v1, const ShapeData::vertexType* v2)
//	{
//		//glm::vec3 v1_p = std::get<ShapeData::attr::position>(*v1);
//		//glm::vec3 v2_p = std::get<ShapeData::attr::position>(*v2);
//		//glm::vec2 v1_uv = std::get<ShapeData::attr::uv>(*v1);
//		//glm::vec2 v2_uv = std::get<ShapeData::attr::uv>(*v2);
//		//glm::vec3 v1_n = std::get<ShapeData::attr::normal>(*v1);
//		//glm::vec3 v2_n = std::get<ShapeData::attr::normal>(*v2);
//
//		//return
//		//	VectorUtils::isNear(v1_p, v2_p, DISTANCE_THRESHOLD) &&
//		//	VectorUtils::isNear(v1_uv, v2_uv, DISTANCE_THRESHOLD) &&
//		//	VectorUtils::isNear(v1_n, v2_n, DISTANCE_THRESHOLD);
//		return false;
//	}
//};
