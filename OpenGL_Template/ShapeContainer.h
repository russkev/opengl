#pragma once

#include <vector>
#include <string>
#include <map>
#include <utility>
#include <regex>
#include <sstream>
#include <iomanip>

#include <glm/glm.hpp>
#include "ShapeData.h"


struct ShapeContainer
{
	// // ----- Type Definitions ----- // //
	typedef std::string						nameType;
	typedef std::pair<nameType, ShapeData>	shapeType;
	typedef std::pair<nameType, glm::mat4>	matType;
	typedef std::pair<nameType, nameType>	connectionType;

	void appendShape(ShapeData&& s_shape, const std::string& s_name = "poly");
	void appendTransform(glm::mat4&& s_transform, const std::string& s_name = "transform");
	void connect(const std::string& source, const std::string& destination);

	std::string type(const std::string& s_name);

	// // ----- Getters ----- // //
	ShapeData::verticesType vertices()
	{
		ShapeData::verticesType t_vertices;
		for (auto & i : m_shapes)
		{
			t_vertices.insert(t_vertices.end(), i.second.vert_begin(), i.second.vert_end());
			//t_vertices.push_back(i.second.vertices());
		}
		return t_vertices;
	}
	std::vector<ShapeData::indicesType> indices()
	{
		std::vector<ShapeData::indicesType> t_indices_vec;
		std::size_t numVertices = 0;
		for (auto & i : m_shapes)
		{
			std::vector<ShapeData::indexType> t_indices;
			for (auto j = 0; j < i.second.numIndices(); ++j)
			{
				t_indices.push_back(i.second.indices().at(j) + numVertices);
			}
			numVertices += i.second.numVertices();
			t_indices_vec.push_back(t_indices);
			//t_indices_vec.insert(t_indices.end(), i.second.indx_begin(), i.second.indx_end());
		}
		return t_indices_vec;
	}

		
private:
	bool nameExists(const std::string& s_name);
	void incrementString(std::string& s_name);
	// // ----- Member Variables ----- // //
	std::map<nameType, ShapeData>		m_shapes;
	std::map<nameType, glm::mat4>		m_transforms;
	std::map<nameType, nameType>		m_connections;
};