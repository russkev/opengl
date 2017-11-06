#pragma once

#include <vector>
#include <string>
#include <map>
#include <utility>

#include <glm/glm.hpp>
#include "ShapeData.h"


struct ShapeContainer
{
	// // ----- Type Definitions ----- // //
	typedef std::string						nameType;
	typedef std::pair<nameType, ShapeData>	shapeType;
	typedef std::pair<nameType, glm::mat4>	matType;
	typedef std::pair<nameType, nameType>	connectionType;

	bool nameExists(const std::string& s_name)
	{
		return
			(
				m_shapes_map.find(s_name)		!= m_shapes_map.end() ||
				m_transforms_map.find(s_name)	!= m_transforms_map.end()
				);
	}

	void appendShape(ShapeData&& s_shape, const std::string& s_name)
	{
		auto t_name = s_name;
		if (nameExists(s_name))
		{
			t_name = "error";
		}

		m_shapes.push_back(std::move(s_shape));
		m_shapes_map.insert(std::pair<nameType, GLuint>(t_name, m_shapes.size() - 1));
	}
private:
	// // ----- Member Variables ----- // //
	std::vector<ShapeData>			m_shapes;
	std::vector<glm::mat4>			m_transforms;
	std::map<nameType, GLuint>		m_shapes_map;
	std::map<nameType, GLuint>		m_transforms_map;
	std::map<nameType, nameType>	m_connections;
};