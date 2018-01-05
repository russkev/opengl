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
	typedef std::string									nameType;
	typedef GLuint										intType;
	typedef std::pair<nameType, ShapeData>				shapeType;
	typedef std::pair<nameType, glm::mat4>				matType;
	typedef std::pair<nameType, std::vector<nameType>>	connectionType;

	void appendShape(ShapeData&& s_shape, const std::string& s_name = "poly");
	void appendTransform(glm::mat4&& s_transform, const std::string& s_name = "transform");
	void connect(const std::string& source, const std::string& destination);
	std::string input(const std::string& destination, const std::string& s_destination_type);
	bool sourceConnectionExists(const std::string& source);
	void transform(glm::mat4& sourceMatrix, const glm::mat4& transformMatrix);
	void transform(ShapeData::verticesType&, const glm::mat4& transformMatrix);
	template <typename T>
	void transformFromConnection(const std::string& name, T& s_object)
	{
		{
			std::string sourceMatrixName = input(name);
			if (sourceMatrixName != "")
			{
				glm::mat4 sourceMatrix = m_transforms.at(sourceMatrixName);
				transformFromConnection(sourceMatrixName, sourceMatrix);
				transform(s_object, sourceMatrix);
			}
		}
	}


	std::string type(const std::string& s_name);

	// // ----- Getters ----- // //
	ShapeData::verticesType vertices();
	ShapeData::indicesType  indices();

	intType findString(const std::vector<std::string> &s_vec, const std::string &s_string);
		
private:
	bool nameExists(const std::string& s_name);
	void incrementString(std::string& s_name);
	// // ----- Member Variables ----- // //
	std::vector<ShapeData>		m_shapes;
	std::vector<nameType>		m_shape_names;
	std::vector<glm::mat4>		m_transforms;
	std::vector<nameType>		m_transform_names;
	std::vector<glm::ivec3>		m_connections; // [source transform, destination transform (if any), destination shape (if any)]
};