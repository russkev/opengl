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

	bool nameExists(const std::string& s_name)
	{
		return
			(	m_shapes.find(s_name)		!= m_shapes.end() ||
				m_transforms.find(s_name)	!= m_transforms.end()	);
	}

	void appendShape(ShapeData&& s_shape, const std::string& s_name)
	{
		std::string t_name = std::string(s_name);

		std::regex re("[^0-9]+([0-9]+$)");
		std::smatch sm;
		if (std::regex_match(t_name, sm, re))
		{
			auto length = sm[1].length();
			std::string s2 = "plane_01";
			for (auto i = 0; i < length; ++i)
			{
				s2.pop_back();
				t_name.pop_back();
			}

			auto num = stoi(sm[1]);

			auto i = 0;
		}

		

		if (nameExists(s_name))	{
			t_name = "error";
		}

		m_shapes.insert(std::pair<nameType, ShapeData>(t_name, std::move(s_shape)));
	}
private:
	// // ----- Member Variables ----- // //
	std::map<nameType, ShapeData>		m_shapes;
	std::map<nameType, glm::mat4>		m_transforms;
	std::map<nameType, nameType>		m_connections;
};