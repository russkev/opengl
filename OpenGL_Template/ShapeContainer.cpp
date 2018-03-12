#include "ShapeContainer.h"
#include "ShapeData.h"
#include "Utilities.h"

void ShapeContainer::appendShape(ShapeData&& s_shape, const std::string& s_name)
{
	auto id = (GLuint)m_shapes.size();
	auto t_name = s_name;

	while ( nameExists(t_name) ) { incrementString(t_name); }

	m_shapes.push_back(std::move(s_shape));
	m_shape_names.push_back(std::move(t_name));
	m_shapes.at(id).setId(id);
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
	assert(sourceLoc >= 0 && destLoc >= 0);
	assert(nameExists(source) && nameExists(destination));

	std::vector<std::string> * destination_strings = NULL;
	if (destType == "shape")		{ destination_strings = &m_shape_names;  }
	if (destType == "transform")	{ destination_strings = &m_transform_names; }

	std::string existingInput	= input(destination, *destination_strings);
	intType existingInputLoc	= findString("transform", existingInput);

	if (existingInput != "") // If destination already has an incoming connection
	{
		for (auto i = 0; i < m_connections.size(); ++i)
		{
			if (connectionExists(i, existingInputLoc, destLoc, destType))
			{
				m_connections.erase(m_connections.begin() + i);
				m_connection_names.erase(m_connection_names.begin() + i);
			}
		}
	}
	m_connections.push_back({ sourceLoc, (destType == "transform") * destLoc + ((destType == "transform") - 1), (destType == "shape") * destLoc + ((destType == "shape") - 1) });
	m_connection_names.push_back(source + " -> " + destination);

	assert(m_connections.size() == m_connection_names.size());
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
	if (findString(m_shape_names, s_name) != -1) { return "shape"; }
	if (findString(m_transform_names, s_name) != -1) { return "transform"; }
	else { return "0"; }
}

bool ShapeContainer::nameExists(const std::string& s_name)
{
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
	for (auto i = 0; i < m_shapes.size(); ++i)
	{
		ShapeData::verticesType shapeVerts = m_shapes.at(i).vertices();
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
			t_indices.push_back(shape.indices().at(j) + (ShapeData::indexType)numVertices);
		}
		numVertices += shape.numVertices();
	}
	return t_indices;
}

std::vector<GLuint> ShapeContainer::inputConnection(const std::string& s_name, std::vector<GLuint> s_existingConnections)
{
	GLint destShapeLoc = -1, destTransformLoc = -1;
	bool destIsShape = false, destIsTransform = false;
	for (auto i = 0; i < m_shapes.size(); ++i)
	{
		if (m_shape_names.at(i) == s_name)
		{
			destShapeLoc = i;
			destIsShape = true;
		}
	}

	for (auto i = 0; i < m_transforms.size(); ++i)
	{
		if (m_transform_names.at(i) == s_name)
		{
			destShapeLoc = i;			destIsTransform = true;
		}
	}

	if (!destIsTransform && !destIsShape)
	{
		return s_existingConnections;
	}

	for (auto & connection : m_connections)
	{
		if (connection[shapeDest] == destShapeLoc && destShapeLoc > -1 && destIsShape)
		{
			s_existingConnections.push_back(connection[transformSource]);
			return inputConnection(m_transform_names[connection[transformSource]], s_existingConnections);
		}
		if (connection[transformDest] == destTransformLoc && destTransformLoc > -1 && destIsTransform)
		{
			s_existingConnections.push_back(connection[transformSource]);
			return inputConnection(m_transform_names[connection[transformSource]], s_existingConnections);
		}
	}
	return s_existingConnections;
}

ShapeData::indicesType ShapeContainer::depthSort(glm::vec3 s_cam_location)
{
	std::vector<std::pair<GLfloat, GLuint>> distances;
	ShapeData::indicesType outIndices;

	for (auto shapeID = 0; shapeID < m_shape_names.size(); ++shapeID)
	{
		auto input				= inputConnection(m_shape_names.at(shapeID));
		//std::vector<GLint> input	= { 0, 1, 2 };
		glm::mat4 matCombined	= glm::mat4(1.0f);
		
		for (auto rit = input.rbegin(); rit != input.rend(); rit++)
		{
			matCombined *= m_transforms.at(*rit);
		}
		glm::vec3 positionVector	= { matCombined[3].x, matCombined[3].y, matCombined[3].z };
		GLfloat distance			= Utilities::distanceSquared(positionVector, s_cam_location);

		distances.push_back(std::make_pair(distance, shapeID));

	}

	Utilities::quickSortPairVector(distances);
	
	
	
	
	//distancesType distances;
	//auto raw_indices = indices();
	//auto raw_vertices = vertices();
	//ShapeData::indicesType outIndices;
	//for (auto i = 0; i < raw_indices.size(); i += 3)
	//{
	//	auto t_vertex_1 = std::get<ShapeData::attr::position>(raw_vertices.at(raw_indices.at(i)));
	//	auto t_vertex_2 = std::get<ShapeData::attr::position>(raw_vertices.at(raw_indices.at(i + 1)));
	//	auto t_vertex_3 = std::get<ShapeData::attr::position>(raw_vertices.at(raw_indices.at(i + 2)));

	//	auto t_vertex_loc_avg = Utilities::vectorAverage(t_vertex_1, t_vertex_2, t_vertex_3);
	//	GLfloat distance = Utilities::distanceSquared(t_vertex_loc_avg, s_cam_location);

	//	distances.push_back(std::make_pair(distance, glm::tvec3<ShapeData::indexType>{raw_indices.at(i), raw_indices.at(i + 1), raw_indices.at(i + 2)}));
	//}
	//Utilities::quickSortPairVector(distances);

	for (auto & i : distances)
	{
		auto numIndices = 0;
		for (auto j = 0; j < i.second; ++j) 
		{
			numIndices += m_shapes.at(j).sizeIndices();
		}
		auto shape = std::move(m_shapes.at(i.second));
		for (auto it = shape.indx_begin(); it != shape.indx_end(); ++it)
		{
			outIndices.push_back(*it + numIndices);
		}
	}
	//auto outIndices = std::move(m_shapes.at(0));
	return outIndices;
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

bool ShapeContainer::connectionExists(const intType s_connections_location, const intType s_source_location, const intType s_dest_location, const std::string& s_dest_type)
{
	if (s_dest_type == "transform")
	{
		return m_connections.at(s_connections_location)[0] == s_source_location &&
			m_connections.at(s_connections_location)[1] == s_dest_location;
	}
	if (s_dest_type == "shape")
	{
		return m_connections.at(s_connections_location)[0] == s_source_location &&
			m_connections.at(s_connections_location)[2] == s_dest_location;
	}
	return false;
}

void ShapeContainer::uploadTransforms(const intType s_program_id)
{
	if (m_transforms.size() > 0) 
	{
		glUseProgram(s_program_id);
		const intType numTransforms = (intType)m_transforms.size();
		const intType transformsLocation = glGetUniformLocation(s_program_id, "transforms");
		glUniformMatrix4fv(transformsLocation, numTransforms, GL_FALSE, &m_transforms.at(0)[0][0]);
	}
}

void ShapeContainer::uploadConnections(const intType s_program_id)
{
	if (m_connections.size() > 0)
	{
		glUseProgram(s_program_id);
		const intType numConnections = (intType)m_connections.size();
		const intType connectionsLocation = glGetUniformLocation(s_program_id, "connections");
		glUniform3iv(connectionsLocation, numConnections, &m_connections.at(0)[0]);
	}
}