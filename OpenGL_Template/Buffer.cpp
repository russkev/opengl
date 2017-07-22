#pragma once
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <cassert>
#include <GL/glew.h>

#include "Buffer.h"


// DEFAULT CONSTRUCTOR //
Buffer::Buffer(std::uint32_t target, std::size_t size) :
	m_size(size),
	m_target(target)
{
	std::cout << "Constructor\n";
	if (m_size > 0u) {
		glGenBuffers(1, &m_bufferID);
		assert(m_target != 0);
		glBindBuffer(m_target, m_bufferID);
		glBufferData(m_target, m_size, nullptr, GL_DYNAMIC_DRAW);
	}
};

// DESTRUCTOR //
Buffer::~Buffer() {
	std::cout << "Destructor\n";

	glDeleteBuffers(1, &m_bufferID);
}

// MOVE CONSTRUCTOR //
Buffer::Buffer(Buffer&& other) :
	m_size(std::exchange(other.m_size, 0u))
{
	std::cout << "Move constructor\n";
}

// MOVE ASSIGN
Buffer& Buffer::operator=(Buffer&& other)
{
	//if (other.m_size != 0u) {
		m_size = std::exchange(other.m_size, 0u);
		std::cout << "Move assign";
	//}
	return *this;
}