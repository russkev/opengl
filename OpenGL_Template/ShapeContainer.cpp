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
	//transform(destination, m_transforms.at(source));
}

//void ShapeContainer::transform(const std::string& name, const glm::mat4& transformMatrix)
//{
//	auto t_type = type(name);
//	if (t_type == "shape")
//	{
//		m_shapes.at(name).transform(transformMatrix);
//	}
//	if (t_type == "transform")
//	{
//		glm::mat4 newMatrix = transformMatrix * m_transforms.at(name);
//		m_transforms.at(name) = newMatrix;
//		for (auto & i : m_connections)
//		{
//			if (i.first == name)
//			{
//				transform(i.second, newMatrix);
//			}
//		}
//	}
//}



//void ShapeContainer::tranformFromConnection(const std::string& name)
//{
//
//}

void ShapeContainer::transform(glm::mat4& sourceMatrix, const glm::mat4& transformMatrix)
{
	sourceMatrix = transformMatrix * sourceMatrix;
}

void ShapeContainer::transform(ShapeData::verticesType& sourceVerts, const glm::mat4& transformMatrix)
{
	ShapeData t_shape;
	t_shape.transform(sourceVerts, transformMatrix);
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
		std::size_t num_length		= sm[1].length();
		auto name_length			= s_name.length();
		auto num					= stoi(sm[1]);
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
// // ----- Getters ----- // //
ShapeData::verticesType ShapeContainer::vertices()
{
	/*
	make vector of vertices, check for input as you go
	if input exists, staroe matrix, check for input
	do this recursivelly until no input exists
	apply matrix transforms
	store transformed vertices
	go through connections list and apply transform to the appropriate vertices
	
	*/
	ShapeData::verticesType t_vertices;

	for (auto & shape : m_shapes)
	{
		ShapeData::verticesType shapeVerts = shape.second.vertices();
		//auto input = matInput(shape.first);
		//if (input != nullptr)
		//{
		//	ShapeData t_shape;
		//	t_shape.transform(shapeVerts, *input);
		//}
		tranformFromConnection(shape.first, shapeVerts);
		t_vertices.insert(t_vertices.end(), shapeVerts.begin(), shapeVerts.end());
	}
	//if (auto input matInput())

	//for (auto & i : m_shapes)
	//{
	//	t_vertices.insert(t_vertices.end(), i.second.vert_begin(), i.second.vert_end());
	//}
	return t_vertices;
}
ShapeData::indicesType ShapeContainer::indices()
{
	ShapeData::indicesType t_indices;
	std::size_t numVertices = 0;
	for (auto & i : m_shapes)
	{
		for (auto j = 0; j < i.second.numIndices(); ++j)
		{
			t_indices.push_back(i.second.indices().at(j) + numVertices);
		}
		numVertices += i.second.numVertices();
	}
	return t_indices;
}

glm::mat4* ShapeContainer::matInput(const std::string& destination)
{
	for (auto & connection : m_connections)
	{
		if (connection.second == destination)
		{
			return &(m_transforms.at(connection.first));
		}
	}
	return nullptr;
}

std::string ShapeContainer::input(const std::string& destination)
{
	std::string source = "";
	for (auto & connection : m_connections)
	{
		if (connection.second == destination)
		{
			source = connection.first;
			break;
		}
	}
	return source;
}