#include "ShapeContainer.h"

void ShapeContainer::appendShape(ShapeData&& s_shape, const std::string& s_name)
{
	auto id = m_shapes.size();
	auto t_name = s_name;
	//auto suffix = "_shape";

	while (nameExists(t_name) /*|| nameExists(t_name + suffix)*/) { incrementString(t_name); }

	m_shapes.push_back(std::move(s_shape));
	m_shape_names.push_back(std::move(t_name));
	m_shapes.at(id).setId(id);
	//appendTransform(glm::mat4(), t_name);
	//connect(t_name, t_name+suffix);
	assert(m_shapes.size() == m_shape_names.size());
}

void ShapeContainer::appendTransform(glm::mat4&& s_transform, const std::string& s_name)
{
	auto t_name = s_name;
	while (nameExists(t_name))	{ incrementString(t_name); }

	m_transforms.push_back(std::move(s_transform));
	m_transform_names.push_back(std::move(t_name));

	assert(m_transforms.size() == m_transform_names.size());
}

void ShapeContainer::connect(const std::string& source, const std::string& destination)
{
	auto sourceType		= type(source);
	auto sourceLoc		= findString(sourceType, source);
	auto destType		= type(destination);
	auto destLoc		= findString(destType, destination);

	assert(sourceType != "0" && destType != "0");
	assert(sourceType == "transform");
	assert(sourceLoc > -1 && destLoc > -1);

	std::vector<std::string> * destination_strings = NULL;
	if (destType == "shape")		{ destination_strings = &m_shape_names;  }
	if (destType == "transform")	{ destination_strings = &m_transform_names; }

	std::string existingInput	= input(destination, *destination_strings);
	intType existingInputLoc	= findString("transform", existingInput);

	if (!sourceConnectionExists(sourceLoc))	
	{ 
		//m_connections.insert(connectionType(source, {})); 
		m_connections.push_back({ sourceLoc, (destType == "transform") * destLoc, (destType == "shape") * destLoc });
	}

	if (existingInput != "") // If destination already has an incoming connection
	{
		//std::size_t vecSize = m_connections.at(existingInput).size();
		auto vecSize = numDestinations(sourceLoc);

		// // !!! Up to here
		for (auto i = 0; i < m_connections.size(); ++i)
		{
			//if (m_connections.at(existingInput).at(i) == destination) // Remove the existing connection from vector
			//if (m_connections.at(i)[0] == existingInputLoc && (m_connections.at(i)[1] == destLoc || m_connections.at(i)[2] == destLoc))
			if (connectionExists(i, existingInputLoc, destLoc))
			{
				//m_connections.at(existingInput).erase(m_connections.at(existingInput).begin() + i);
				m_connections.erase(m_connections.begin() + i);
			}
			//if (m_connections.at(existingInput).size() == 0) // Remove connection from m_connections if it was the only connection
			//{
			//	m_connections.erase(existingInput);
			//}
		}
	}
	//if (m_connections.find(source) != m_connections.end())
	//{
	//	m_connections.at(source).push_back(destination);
	//}
}

std::string ShapeContainer::input(const std::string& s_destination, const std::vector<std::string>& s_destination_strings)
{
	intType location = findString(s_destination_strings, s_destination);
	for (auto & connection : m_connections)
	{
		if (connection[1] == location || connection[2] == location)
		{
			return m_transform_names.at(connection[0]);
		}
	}
	return "";
}

bool ShapeContainer::sourceConnectionExists(const intType source)
{
	for (auto i = 0; i < m_connections.size(); ++i)
	{
		if (m_connections.at(i)[0] == source) { return true; }
	}
	//return m_connections.find(source) != m_connections.end();
	return false;
}

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
	//if (m_shapes.find(s_name) != m_shapes.end()) { return "shape"; }
	if (findString(m_shape_names, s_name) != -1) { return "shape"; }
	//if (m_transforms.find(s_name) != m_transforms.end()) { return "transform"; }
	if (findString(m_transform_names, s_name) != -1) { return "transform"; }
	else { return "0"; }
}

bool ShapeContainer::nameExists(const std::string& s_name)
{
	//return
	//	(m_shapes.find(s_name) != m_shapes.end() ||
	//		m_transforms.find(s_name) != m_transforms.end());
	for (auto & shape_name : m_shape_names)
	{
		if (shape_name == s_name) { return true; }
	}
	for (auto & transform_name : m_transform_names)
	{
		if (transform_name == s_name) { return true; }
	}
	return false;
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

	//for (auto & shape : m_shapes)
	for (auto i = 0; i < m_shapes.size(); ++i)
	{
		ShapeData::verticesType shapeVerts = m_shapes.at(i).vertices();
		transformFromConnection(m_shape_names.at(i), shapeVerts);
		t_vertices.insert(t_vertices.end(), shapeVerts.begin(), shapeVerts.end());
	}
	return t_vertices;
}


ShapeData::indicesType ShapeContainer::indices()
{
	ShapeData::indicesType t_indices;
	std::size_t numVertices = 0;
	for (auto & shape : m_shapes)
	{
		for (auto j = 0; j < shape.numIndices(); ++j)
		{
			t_indices.push_back(shape.indices().at(j) + numVertices);
		}
		numVertices += shape.numVertices();
	}
	return t_indices;
}

ShapeContainer::intType ShapeContainer::findString(const std::vector<std::string> &s_vec, const std::string &s_string)
{
	assert(s_vec.size() > 0);
	for (intType i = 0; i < s_vec.size(); ++i)
	{
		if (s_vec.at(i) == s_string) { return i; }
	}
	return (intType)-1;
}

ShapeContainer::intType ShapeContainer::findString(const std::string &s_type, const std::string &s_string)
{
	if (s_type == "shape")		{ return findString(m_shape_names,		s_string); }
	if (s_type == "transform")	{ return findString(m_transform_names,	s_string); }
	return (intType)-1;
}

ShapeContainer::intType ShapeContainer::numDestinations(const ShapeContainer::intType s_source)
{
	intType destinations = 0;
	for (auto & connection : m_connections)
	{
		if (connection[0] == s_source) { ++destinations; }
	}
	return destinations;
}

bool ShapeContainer::connectionExists(const intType s_location, const intType s_source_location, const intType s_dest_location)
{
	return
		m_connections.at(s_location)[0] == s_source_location &&
		(
			m_connections.at(s_location)[1] == s_dest_location || 
			m_connections.at(s_location)[2] == s_dest_location
			);
}

void ShapeContainer::uploadTransforms(const intType s_program_id)
{
	glUseProgram(s_program_id);
	const intType numTransforms = m_transforms.size();
	const intType transformsLocation = glGetUniformLocation(s_program_id, "transforms");
	glUniformMatrix4fv(transformsLocation, numTransforms, GL_FALSE, &m_transforms.at(0)[0][0]);
	//glUseProgram(_State.programID);
	//const GLuint numTransforms = _State.sh.numTransforms();
	//const GLint transformsLocation = glGetUniformLocation(_State.programID, "transforms");
	//std::vector<glm::mat4> transformsToUpload = _State.sh.transforms();
	//glUniformMatrix4fv(transformsLocation, numTransforms, GL_FALSE, &transformsToUpload[0][0][0]);
}