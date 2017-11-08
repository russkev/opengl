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

	void incrementString(std::string& s_name)
	{
		std::regex re("[^0-9]+([0-9]+$)");
		std::smatch sm;
		//if (!std::regex_match(s_name, sm, re))
		//{
		//	s_name = s_name + "_01";
		//}
		//auto num_length = sm[1].length();
		//auto num = stoi(sm[1]);
		//if (std::to_string(num + 1).length() > num_length)
		//{
		//	++num_length;
		//}

		//std::ostringstream t_name_stream;
		//t_name_stream << s_name;
		//t_name_stream.seekp(s_name.length() - num_length);
		//t_name_stream << std::setfill('0') << std::setw(num_length) << ++num;
		//s_name = t_name_stream.str();
	}

	void appendShape(ShapeData&& s_shape, const std::string& s_name)
	{
		auto t_name = s_name;
		while (nameExists(t_name))
		{
			incrementString(t_name);
		}

		m_shapes.insert(std::pair<nameType, ShapeData>(t_name, std::move(s_shape)));
	}
private:
	// // ----- Member Variables ----- // //
	std::map<nameType, ShapeData>		m_shapes;
	std::map<nameType, glm::mat4>		m_transforms;
	std::map<nameType, nameType>		m_connections;
};