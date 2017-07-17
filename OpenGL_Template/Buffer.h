#pragma once

#include <GL\glew.h>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "Camera.h"
#include "ShapeData.h"

struct Buffer
{
	// // Constructor // //
	Buffer(std::uint32_t target_, std::size_t initial_length_);

	// // Functions // //
	void addShape(const ShapeData& shape, const glm::mat4 matrix = glm::mat4(1.0f));
	void addShape(const ShapeData& shape, const std::vector<glm::mat4>& matrices);
	void createGeoBuffer();
	void createMatrixBuffer(const void* data, std::size_t size, std::uint32_t attribute, std::uint32_t vertexArrayID);
	void drawGeo(const Camera& cam, const glm::mat4& projection);
	std::uint32_t getBufferID();
	std::uint32_t Buffer::getViewMatrixBufferID();
	std::uint32_t Buffer::getWorldMatrixBufferID();
private:
	std::uint32_t m_target;
	std::size_t m_length;
	std::uint32_t m_vertexBufferID;
	std::uint32_t m_viewMatrixBufferID;
	std::uint32_t m_worldMatrixBufferID;
	std::vector<std::uint32_t> m_arrayIDs;
	std::size_t m_bufferSize;
	std::vector<std::size_t> m_instances;
	std::vector<glm::mat4>   m_matrices;
	std::vector<std::size_t> m_vertexSizes;
	std::vector<std::size_t> m_indiceSizes;
	std::vector<std::size_t> m_indice_numbers;
	std::vector<ShapeData> m_shapes;

};
