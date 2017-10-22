#pragma once

#include <vector>

#include "Buffer.h"

#include "GL_Tuple_Introspect.h"



struct VAO
{
	// // BIG 6
	VAO(/*const std::uint32_t target*/);

	// // GENERAL FUNCTIONS
	void Append(const std::uint32_t attribute, const std::size_t numElements, const std::size_t elementSize, const std::uint32_t type);
	void GenerateID(const Buffer& inBuffer);
	void GenerateVAO(const Buffer& inBuffer, std::size_t divisor, const member_info_type* begin, const member_info_type* end, std::uint32_t id_offset =  0u);
	void ClearVectors();
	void Bind();
	
	//void BindBuffer(const Buffer& inputBuffer);

	// // GETTERS
	std::uint32_t VAO_ID() { return m_VAO_ID; }

private:
	std::uint32_t					m_target;
	std::uint32_t					m_VAO_ID;
	std::vector<std::uint32_t>		m_attributes;
	std::vector<std::size_t>		m_elementAmounts;
	std::vector<std::size_t>		m_elementSizes;
	std::vector<std::uint32_t>		m_types;
};