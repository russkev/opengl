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
	//void transform(const std::string& name, const glm::mat4& transformMatrix);
	void transform(glm::mat4& sourceMatrix, const glm::mat4& transformMatrix);
	void transform(ShapeData::verticesType&, const glm::mat4& transformMatrix);
	template <typename T>
	void tranformFromConnection(const std::string& name, T& s_object)
	{
		auto source = input(name);
		if (source != "")
		{
			transform(s_object, source);
			transformFromConnection(source, s_object);
		}
	}

	glm::mat4* matInput(const std::string& destination);
	std::string input(const std::string& name);
	//void transformTransform(const std::string& transformName, const glm::mat4& transformMatrix);


	std::string type(const std::string& s_name);

	// // ----- Getters ----- // //
	ShapeData::verticesType vertices();
	ShapeData::indicesType  indices();

		
private:
	bool nameExists(const std::string& s_name);
	void incrementString(std::string& s_name);
	// // ----- Member Variables ----- // //
	std::map<nameType, ShapeData>		m_shapes;
	std::map<nameType, glm::mat4>		m_transforms;
	std::map<nameType, nameType>		m_connections;
};