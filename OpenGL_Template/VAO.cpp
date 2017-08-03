#pragma once
#include <iostream>
#include <cassert>
#include <GL/glew.h>

#include "VAO.h"

// // CONSTRUCTOR //
VAO::VAO(/*const std::uint32_t target*/) :
	m_VAO_ID(0)
//	m_target(target)
{
	m_VAO_ID = GenerateVAO();
	std::cout << "VAO object created\n";
	// SOMETHING
};

// // APPEND
void VAO::Append(const std::uint32_t attribute, const std::size_t numElements, const std::size_t elementSize) {
	assert(m_attributes.size() == m_sizes.size());
	glBindVertexArray(m_VAO_ID);
	glEnableVertexAttribArray(attribute);
	std::size_t offset = 0;
	if (m_sizes.size() != 0) {
		for (auto i : m_sizes) {
			offset += i;
		}
	}
	glVertexAttribPointer(attribute, numElements, GL_FLOAT, GL_FALSE, elementSize, (void*)offset);
}

// // GENERATE BUFFER
std::uint32_t VAO::GenerateVAO() {
	std::uint32_t tempID = 0;
	glGenVertexArrays(1, &tempID);
	return tempID;
}
