#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

struct Buffer
{
	Buffer(std::uint32_t target_, std::size_t initial_length_);
	void createGeoBuffer(const void* data, std::size_t size);
	void createMatrixBuffer(const void* data, std::size_t size);
private:
	void createBuffer(const void* data, std::size_t size);
	void createVAOs(const void* data, std::vector<std::uint32_t> attributes, std::uint32_t step, std::uint32_t initialOffset);
	std::uint32_t m_target;
	std::size_t m_length;
	//std::size_t m_size;
	std::uint32_t m_bufferID;

};
