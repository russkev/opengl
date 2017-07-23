#pragma once

#include <GL\glew.h>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "Camera.h"
#include "ShapeData.h"

struct GLShapes
{
	// // Constructor // //
	GLShapes(std::uint32_t target_);

	// // Destructor // //
	~GLShapes();

	// // Functions // //
	void addShape(const ShapeData& shape, const glm::mat4 matrix = glm::mat4(1.0f));
	void addShape(const ShapeData& shape, const std::vector<glm::mat4>& matrices);
	void createGeoBuffer();
	void drawGeo(const Camera& cam, const glm::mat4& projection, std::uint32_t mode);
private:
	void createMatrixBuffer(const void* data, std::size_t size, std::uint32_t attribute, std::uint32_t vertexArrayID);
	std::uint32_t m_target;
	std::uint32_t m_vertexBufferID;
	std::uint32_t m_viewMatrixBufferID;
	std::uint32_t m_worldMatrixBufferID;
	std::vector<std::uint32_t> m_arrayIDs;
	std::size_t m_bufferSize;
	std::size_t m_matrixBufferSize;
	std::vector<std::size_t> m_instances;
	std::vector<glm::mat4>   m_matrices;
	std::vector<std::size_t> m_vertexSizes;
	std::vector<std::size_t> m_indiceSizes;
	std::vector<std::size_t> m_indice_numbers;
	std::vector<ShapeData> m_shapes;

};