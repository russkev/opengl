#pragma once
#include <iostream>
#include <cassert>
#include <GL/glew.h>

#include "Buffer.h"
#include "VAO.h"

// // CONSTRUCTOR //
VAO::VAO(/*const std::uint32_t target*/) :
	m_VAO_ID(0)
//	m_target(target)
{
	//m_VAO_ID = GenerateVAO();
	std::cout << "VAO object created\n";
	// SOMETHING
};

// // APPEND
void VAO::Append(const std::uint32_t attribute, const std::size_t numElements, const std::size_t elementSize, const std::uint32_t type) {
	//assert(m_attributes.size() == m_sizes.size());
	//glBindVertexArray(m_VAO_ID);
	//glEnableVertexAttribArray(attribute);
	//std::size_t offset = 0;
	//if (m_sizes.size() != 0) {
	//	for (auto i : m_sizes) {
	//		offset += i;
	//	}
	//}
	//glVertexAttribPointer(attribute, numElements, GL_FLOAT, GL_FALSE, elementSize, (void*)offset);
	m_attributes.push_back(attribute);
	m_elementAmounts.push_back(numElements);
	m_elementSizes.push_back(elementSize);
	m_types.push_back(type);
}

void VAO::GenerateVAO() {
	assert(m_attributes.size() == m_elementAmounts.size() && m_attributes.size() == m_elementSizes.size());
	glBindVertexArray(m_VAO_ID);
	std::size_t offset = 0;
	std::size_t stride = 0;
	for (int i = 0; i < m_attributes.size(); ++i) {
		stride += m_elementSizes.at(i) * m_elementAmounts.at(i);
	}
	for (int i = 0; i < m_attributes.size(); ++i) {
		glEnableVertexAttribArray(m_attributes.at(i));
		glVertexAttribPointer(m_attributes.at(i), m_elementAmounts.at(i), m_types.at(i), GL_FALSE, stride, (void*)offset);
		offset += m_elementSizes.at(i) * m_elementAmounts.at(i);
	}
}

// // GENERATE ID
void VAO::GenerateID(const Buffer& inputBuffer) {
	inputBuffer.Bind();
	glGenVertexArrays(1, &m_VAO_ID);
}

//// // BIND BUFFER
//void VAO::BindBuffer(const Buffer& inputBuffer) {
//	inputBuffer.Bind();
//}
