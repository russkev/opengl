#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

struct Buffer
{
	Buffer(std::size_t);
	~Buffer();
	Buffer(const Buffer&) = delete;
	Buffer(Buffer&&);
	Buffer& operator = (const Buffer&) = delete;
	Buffer& operator = (Buffer&&);
private:
	std::size_t m_size;


};