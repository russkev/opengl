#pragma once
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <cassert>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "Buffer.h"

namespace gl_engine
{
	// DEFAULT CONSTRUCTOR //
	Buffer::Buffer(std::uint32_t target, std::size_t size, std::uint32_t binding) :
		m_size(0),
		m_capacity(size),
		m_buffer_id(0),
		m_target(target),
		m_binding_id(binding)
	{
		std::cout << "Buffer object constructed\n";
		if (size != 0) {
			m_buffer_id = generate_buffer(size);
		}
	};

	// DESTRUCTOR //
	Buffer::~Buffer() {
		std::cout << "Buffer object destructed\n";
		if (m_buffer_id != 0) {
			glDeleteBuffers(1, &m_buffer_id);
		}
	}

	// MOVE CONSTRUCTOR //
	Buffer::Buffer(Buffer&& other) :
		m_size(std::exchange(other.m_size, 0u)),
		m_capacity(std::exchange(other.m_capacity, 0u)),
		m_target(std::exchange(other.m_target, 0u)),
		m_buffer_id(std::exchange(other.m_buffer_id, 0u)),
		m_binding_id(std::exchange(other.m_binding_id, 0u))
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
		void * dest = map_buffer(size, offset);
		std::memcpy(dest, data, size);
		unmap();
	}

	// // GENERATE BUFFER
	std::uint32_t Buffer::generate_buffer(std::size_t size) {
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
	void Buffer::read_buffer(void* destination) {
		void * src = map_buffer(m_size, 0);
		std::memcpy(destination, src, m_size);
		unmap();
	}

	// // MAP
	void * Buffer::map_buffer(std::size_t size, std::size_t offset) {
		assert(offset + size <= m_capacity);
		glBindBuffer(m_target, m_buffer_id);
		if (m_target == GL_ATOMIC_COUNTER_BUFFER)
		{
			glBindBufferBase(m_target, m_binding_id, m_buffer_id);
		}
		else
		{
			glBindBuffer(m_target, m_buffer_id);
		}
		void * out = glMapBufferRange(m_target, offset, size, GL_MAP_WRITE_BIT);
		return out;
	}

	// // UNMAP
	void Buffer::unmap() {
		glBindBuffer(m_target, m_buffer_id);
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
	void Buffer::resize(std::size_t new_size) {
		if (new_size > m_capacity) {
			std::uint32_t tempID = generate_buffer(new_size);
			if (m_buffer_id != 0) {
				glBindBuffer(GL_COPY_READ_BUFFER, m_buffer_id);
				glBindBuffer(GL_COPY_WRITE_BUFFER, tempID);
				glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, m_capacity);

				std::swap(tempID, m_buffer_id);
				glDeleteBuffers(1, &tempID);
			}
			else {
				m_buffer_id = tempID;
			}
			m_capacity = new_size;
		}
		m_size = new_size;
	}

	// // RESERVE
	void Buffer::reserve(std::size_t new_capacity) {
		auto currentSize = m_size;
		if (new_capacity > m_capacity) {
			resize(new_capacity);
			resize(currentSize);
		}
	}

	// // BIND
	void Buffer::bind() const {
		glBindBuffer(m_target, m_buffer_id);
	}

	void Buffer::bind(std::uint32_t target) {
		glBindBuffer(m_target = target, m_buffer_id);
	}

	void Buffer::set_binding_id(std::uint32_t binding_id) {
		m_binding_id = binding_id;
	}
} // namespace gl_engine