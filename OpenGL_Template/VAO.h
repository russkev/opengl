#pragma once

#include <vector>


struct VAO
{
	// // BIG 6
	VAO(/*const std::uint32_t target*/);

	// // GENERAL FUNCTIONS
	void Append(const std::uint32_t attribute, const std::size_t numElements, const std::size_t elementSize, const std::uint32_t type);
	void GenerateID(const Buffer& inputBuffer);
	void GenerateVAO();
	
	//void BindBuffer(const Buffer& inputBuffer);

private:
	std::uint32_t					m_target;
	std::uint32_t					m_VAO_ID;
	std::vector<std::uint32_t>		m_attributes;
	std::vector<std::size_t>		m_elementAmounts;
	std::vector<std::size_t>		m_elementSizes;
	std::vector<std::uint32_t>		m_types;
};