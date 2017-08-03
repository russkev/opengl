#pragma once

#include <vector>


struct VAO
{
	// // BIG 6
	VAO(/*const std::uint32_t target*/);

	// // GENERAL FUNCTIONS
	void Append(const std::uint32_t attribute, const std::size_t numElements, const std::size_t elementSize);
	std::uint32_t GenerateVAO();

private:
	std::uint32_t					m_target;
	std::uint32_t					m_VAO_ID;
	std::vector<std::uint32_t>		m_attributes;
	std::vector<std::size_t>		m_sizes;
};