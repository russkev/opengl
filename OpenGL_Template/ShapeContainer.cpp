#include "ShapeContainer.h"

void ShapeContainer::appendShape(ShapeData&& s_shape, const std::string& s_name)
{
	auto t_name = s_name;
	auto suffix = "_shape";
	//auto t_name_s = s_name + suffix;


	while (nameExists(t_name) || nameExists(t_name + suffix)) { incrementString(t_name); }

	m_shapes.insert(shapeType(t_name + suffix, std::move(s_shape)));
	appendTransform(glm::mat4(), t_name);
	connect(t_name, t_name+suffix);
}

void ShapeContainer::appendTransform(glm::mat4&& s_transform, const std::string& s_name)
{
	auto t_name = s_name;
	while (nameExists(t_name))
	{
		incrementString(t_name);
	}
	m_transforms.insert(matType(t_name, std::move(s_transform)));
}

void ShapeContainer::connect(const std::string& source, const std::string& destination)
{
	auto sourceType = type(source);
	auto destType	= type(destination);
	assert(sourceType != "0" && destType != "0");
	assert(sourceType != "shape");
	m_connections.insert(connectionType(source, destination));
}

std::string ShapeContainer::type(const std::string& s_name)
{
	if (m_shapes.find(s_name) != m_shapes.end()) { return "shape"; }
	if (m_transforms.find(s_name) != m_transforms.end()) { return "transform"; }
	else { return "0"; }
}

bool ShapeContainer::nameExists(const std::string& s_name)
{
	return
		(m_shapes.find(s_name) != m_shapes.end() ||
			m_transforms.find(s_name) != m_transforms.end());
}

void ShapeContainer::incrementString(std::string& s_name)
{
	std::regex re("[^0-9]+([0-9]+$)");
	std::smatch sm;
	if (!std::regex_match(s_name, sm, re))
	{
		s_name = s_name + "_00";
	}
	if (std::regex_match(s_name, sm, re))
	{
		auto num_length		= sm[1].length();
		auto name_length	= s_name.length();
		auto num			= stoi(sm[1]);
		if (std::to_string(num + 1).length() > num_length)
		{
			++num_length;
			++name_length;
		}


		std::ostringstream t_name_stream;
		t_name_stream << s_name;
		t_name_stream.seekp(name_length - num_length);
		t_name_stream << std::setfill('0') << std::setw(num_length) << ++num;
		s_name = t_name_stream.str();
	}
}