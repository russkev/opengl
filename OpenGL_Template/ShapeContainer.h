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
	typedef std::string															nameType;
	typedef GLuint																intType;
	typedef std::pair<nameType, ShapeData>										shapeType;
	typedef std::pair<nameType, glm::mat4>										matType;
	typedef std::pair<nameType, std::vector<nameType>>							connectionType;
	typedef std::vector<std::pair<GLfloat, glm::tvec3<ShapeData::indexType>>>	distancesType;

	void appendShape(ShapeData&& s_shape, const std::string& s_name = "poly");
	void appendTransform(glm::mat4&& s_transform, const std::string& s_name = "transform");
	void connect(const std::string& source, const std::string& destination);
	std::string input(const std::string& destination, const std::vector<std::string>& s_destination_strings);
	bool sourceConnectionExists(const intType source);
	void transform(glm::mat4& sourceMatrix, const glm::mat4& transformMatrix);
	void transform(ShapeData::verticesType&, const glm::mat4& transformMatrix);
	template <typename T>
	void transformFromConnection(const std::string& name, T& s_object)
	{
		{
			//std::string sourceMatrixName = input(name);
			std::string sourceMatrixName = input(name, m_shape_names) + input(name, m_transform_names);
			if (sourceMatrixName != "")
			{
				//glm::mat4 sourceMatrix = m_transforms.at(sourceMatrixName);
				glm::mat4 sourceMatrix = m_transforms.at(findString(m_transform_names, sourceMatrixName));
				transformFromConnection(sourceMatrixName, sourceMatrix);
				transform(s_object, sourceMatrix);
			}
		}
	}


	std::string type(const std::string& s_name);
	ShapeData::indicesType depthSort(glm::vec3 s_cam_location);
	//void quickSortDistances(distancesType& s_distances);

	// // ----- Getters ----- // //
	ShapeData::verticesType vertices();
	ShapeData::indicesType  indices();
	std::vector<glm::mat4>  transforms() { return m_transforms; }
	intType numShapes() { return m_shapes.size(); }
	intType numTransforms() { return m_transforms.size(); }

	intType findString(const std::vector<std::string> &s_vec, const std::string &s_string);
	intType findString(const std::string &s_type, const std::string &s_string);
	intType numDestinations(const intType s_source);
	bool connectionExists(const intType s_location, const intType s_source_location, const intType s_dest_location, const std::string& s_dest_type);

	void uploadTransforms(const intType s_program_id);
	void uploadConnections(const intType s_program_id);

	template <typename T>
	std::vector<T> readUniform(const intType s_program_id, const std::string &s_location_name, const intType numElements = 4)
	{
		auto location = glGetUniformLocation(s_program_id, s_location_name.c_str());
		std::vector<T> returnVector (numElements);
		for (auto i = 0; i < numElements; ++i)
		{
			auto vp = glm::value_ptr(returnVector[i]);
			auto vp_1 = *vp;
			if constexpr(std::is_same_v<decltype(vp_1), float>)
			{
				glGetUniformfv(s_program_id, location + i, vp); 
			}
			else if constexpr(std::is_same_v<decltype(vp_1), int>) 
			{ 
				glGetUniformiv(s_program_id, location + i, vp); 
			}
		}
		return returnVector;
	}
	



private:
	bool nameExists(const std::string& s_name);
	void incrementString(std::string& s_name);
	// // ----- Member Variables ----- // //
	std::vector<ShapeData>		m_shapes;
	std::vector<nameType>		m_shape_names;
	std::vector<glm::mat4>		m_transforms;
	std::vector<nameType>		m_transform_names;
	std::vector<glm::ivec3>		m_connections; // [source transform, destination transform (if any), destination shape (if any)]
	std::vector<std::string>	m_connection_names;
};