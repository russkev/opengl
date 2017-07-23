#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

struct Buffer
{
	Buffer(std::uint32_t target, std::size_t size);
	~Buffer();
	Buffer(const Buffer&) = delete;
	Buffer(Buffer&&);
	Buffer& operator = (const Buffer&) = delete;
	Buffer& operator = (Buffer&&);

	std::uint32_t GenerateBuffer(std::size_t size);
	void Upload(std::size_t, std::size_t, void*);
	void* Map(std::size_t offset, std::size_t size);
	void Unmap();
	std::uint32_t Append(std::size_t size, void* data);
	void Resize(std::size_t newSize);
private:
	std::size_t		m_size;
	std::size_t		m_capacity;
	std::uint32_t	m_bufferID;
	std::uint32_t	m_target;
};