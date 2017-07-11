#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include "Camera.h"

struct Buffer
{
	Buffer(std::uint32_t target_, std::size_t initial_length_);
	void createGeoBuffer(
		const void* vertex_data, std::size_t vertex_size,
		const void* indice_data, std::size_t indice_size,
		std::size_t indice_number);
	void createMatrixBuffer(const void* data, std::size_t size, std::uint32_t attribute, std::uint32_t vertexArrayID);
	void drawGeo(const Camera& cam);
	std::uint32_t getBufferID();
	std::uint32_t getArrayID();
	std::uint32_t Buffer::getViewMatrixBufferID();
	std::uint32_t Buffer::getWorldMatrixBufferID();
private:
	//void createBuffer(const void* data, std::size_t size);
	//void createVAOs(const void* data, std::vector<std::uint32_t> attributes, std::uint32_t step, std::uint32_t initialOffset);
	std::uint32_t m_target;
	std::size_t m_length;
	//std::size_t m_size;
	std::uint32_t m_vertexBufferID;
	std::uint32_t m_viewMatrixBufferID;
	std::uint32_t m_worldMatrixBufferID;
	//std::uint32_t m_arrayBufferID;
	std::uint32_t m_arrayID;
	std::size_t m_vertexSize;
	std::size_t m_indice_number;
};
