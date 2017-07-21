#pragma once
#include "Buffer.h"
#include <cstddef>
#include <cstdint>
#include <iostream>

// DEFAULT CONSTRUCTOR //
Buffer::Buffer(std::size_t size) :
	m_size(size)
{
	std::cout << "Constructor\n";
};

// DESTRUCTOR //
Buffer::~Buffer() {
	std::cout << "Destructor\n";
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
	std::cout << "Move assign";
}