#pragma once
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <cassert>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "Buffer.h"


// DEFAULT CONSTRUCTOR //
Buffer::Buffer(std::uint32_t target, std::size_t size, std::uint32_t binding) :
	m_size(0),
	m_capacity(size),
	m_bufferID(0),
	m_target(target),
	m_bindingID(binding)
{
	std::cout << "Buffer object constructed\n";
	if (size != 0) {
		m_bufferID = generateBuffer(size);
	}
};

// DESTRUCTOR //
Buffer::~Buffer() {
	std::cout << "Buffer object destructed\n";
	if (m_bufferID != 0) {
		glDeleteBuffers(1, &m_bufferID);
	}
}

// MOVE CONSTRUCTOR //
Buffer::Buffer(Buffer&& other) :
	m_size(std::exchange(other.m_size, 0u)),
	m_capacity(std::exchange(other.m_capacity, 0u)),
	m_target(std::exchange(other.m_target, 0u)),
	m_bufferID(std::exchange(other.m_bufferID, 0u)),
	m_bindingID(std::exchange(other.m_bindingID, 0u))
{
	std::cout << "Buffer move constructor called\n";
}

// MOVE ASSIGN
Buffer& Buffer::operator=(Buffer&& other)
{
	std::cout << "Buffer move assign called\n";
	(*this).~Buffer();
	return *new (this) Buffer(std::move(other));
}

// // UPLOAD
void Buffer::upload(std::size_t offset, std::size_t size, void* data) {
	assert(offset + size <= m_capacity);
	void * dest = mapBuffer(size, offset);
	std::memcpy(dest, data, size);
	unmap();
}

// // GENERATE BUFFER
std::uint32_t Buffer::generateBuffer(std::size_t size) {
	std::uint32_t tempID = 0;
	glGenBuffers(1, &tempID);
	assert(tempID != 0u);
	if (size != 0) {
		glBindBuffer(m_target, tempID);
		glBufferData(m_target, size, nullptr, GL_DYNAMIC_DRAW);
	}
	return tempID;
}

// // READ BUFFER
void Buffer::readBuffer(void* dest) {
	void * src = mapBuffer(m_size, 0);
	std::memcpy(dest, src, m_size);
	unmap();
}

// // MAP
void * Buffer::mapBuffer(std::size_t size, std::size_t offset) {
	assert(offset + size <= m_capacity);
	glBindBuffer(m_target, m_bufferID);
	if (m_target == GL_ATOMIC_COUNTER_BUFFER)
	{
		glBindBufferBase(m_target, m_bindingID, m_bufferID);
	}
	else
	{
		glBindBuffer(m_target, m_bufferID);
	}
	void * out = glMapBufferRange(m_target, offset, size, GL_MAP_WRITE_BIT);
	return out;
}

// // UNMAP
void Buffer::unmap() {
	glBindBuffer(m_target, m_bufferID);
	glUnmapBuffer(m_target);
}

// // APPEND
std::uint32_t Buffer::append(std::size_t size, void* data) {
	std::size_t offset = m_size;
	resize(offset + size);
	upload(offset, size, data);
	return (uint32_t)offset;
}

// // RESIZE
void Buffer::resize(std::size_t newSize) {
	if (newSize > m_capacity){
		std::uint32_t tempID = generateBuffer(newSize);
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

// // RESERVE
void Buffer::reserve(std::size_t newCapacity) {
	auto currentSize = m_size;
	if (newCapacity > m_capacity) {
		resize(newCapacity);
		resize(currentSize);
	}
}

// // BIND
void Buffer::bind() const {
	glBindBuffer(m_target, m_bufferID);
}

void Buffer::bind(std::uint32_t target) {
	glBindBuffer(m_target = target, m_bufferID);
}

void Buffer::setBindingID(std::uint32_t bindingID) {
	m_bindingID = bindingID;
}