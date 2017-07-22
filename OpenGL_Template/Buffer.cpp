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
	m_capacity(size),
	m_bufferID(0),
	m_target(target)
{
	std::cout << "Constructor\n";
	if (m_size != 0) {
		m_bufferID = GenerateBuffer(size);
	}
};

// DESTRUCTOR //
Buffer::~Buffer() {
	std::cout << "Destructor\n";
	//if (m_bufferID != 0) {
	//	glDeleteBuffers(1, &m_bufferID);
	//}	
}

// MOVE CONSTRUCTOR //
Buffer::Buffer(Buffer&& other) :
	m_size(std::exchange(other.m_size, 0u)),
	m_capacity(std::exchange(other.m_capacity, 0u)),
	m_target(std::exchange(other.m_target, 0u)),
	m_bufferID(std::exchange(other.m_bufferID, 0u))
{
	std::cout << "Move constructor\n";
}

// MOVE ASSIGN
Buffer& Buffer::operator=(Buffer&& other)
{
		std::cout << "Move assign";
		(*this).~Buffer();
		return *new (this) Buffer(std::move(other));
}

// // UPLOAD
void Buffer::Upload(std::size_t offset, std::size_t size, void* data) {
	//glBufferSubData(m_target, offset, size, data);
	assert(offset + size <= m_capacity);
	void * dest = Map(offset, size);
	std::memcpy(dest, data, size);
	Unmap();
}

// // GENERATE BUFFER
std::uint32_t Buffer::GenerateBuffer(std::size_t size) {
	std::uint32_t tempID = 0;
	glGenBuffers(1, &tempID);
	assert(tempID != 0u);
	if (size != 0) {
		glBindBuffer(m_target, tempID);
		glBufferData(m_target, size, nullptr, GL_DYNAMIC_DRAW);
	}
	return tempID;
}

// // MAP
void * Buffer::Map(std::size_t offset, std::size_t size) {
	assert(offset + size <= m_capacity);
	glBindBuffer(m_target, m_bufferID);
	return glMapBufferRange(m_target, offset, size, GL_MAP_WRITE_BIT);
}

// // UNMAP
void Buffer::Unmap() {
	glBindBuffer(m_target, m_bufferID);
	glUnmapBuffer(m_target);
}

// // RESIZE
void Buffer::Resize(std::size_t newSize) {
	if (newSize > m_capacity){
		std::uint32_t tempID = GenerateBuffer(newSize);
		if (m_bufferID != 0) {
			glBindBuffer(GL_COPY_READ_BUFFER, m_bufferID);
			glBindBuffer(GL_COPY_WRITE_BUFFER, tempID);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, m_capacity);

			std::swap(tempID, m_bufferID);
			glDeleteBuffers(1, &tempID);
		}
		else {
			m_bufferID = tempID;
		}
		m_capacity = newSize;
	}
	m_size = newSize;
}