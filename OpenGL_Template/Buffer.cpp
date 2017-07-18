#include <GL/glew.h>
#include <vector>

#include "Buffer.h"
#include "Vertex.h"
#include "Camera.h"
#include "ShapeData.h"

static constexpr auto POSITION_ATTR = 0u;
static constexpr auto COLOR_ATTR = 1u;
static constexpr auto NORMAL_ATTR = 2u;
static constexpr auto MODEL_ATTR = 4u;
static constexpr auto WORLD_ATTR = 8u;

Buffer::Buffer(std::uint32_t target_) :
	m_target(target_),
	m_vertexBufferID(0),
	m_bufferSize(0)
{
}

Buffer::~Buffer() {
	glDeleteBuffers(1, &m_vertexBufferID);
	glDeleteBuffers(1, &m_viewMatrixBufferID);
	glDeleteBuffers(1, &m_worldMatrixBufferID);
	glDeleteVertexArrays(m_arrayIDs.size(), &m_arrayIDs[0]);
}

void Buffer::addShape(const ShapeData& shape, const glm::mat4 matrix) {
	m_shapes.push_back(shape);
	m_instances.push_back(1);
	m_matrices.push_back(matrix);
}


void Buffer::addShape(const ShapeData& shape, const std::vector<glm::mat4>& matrices) {
	m_shapes.push_back(shape);
	m_instances.push_back(matrices.size());
	for (auto matrix : matrices) { m_matrices.push_back(matrix); }



	return;
}


void Buffer::createGeoBuffer()
{
	for (auto i = 0; i < m_shapes.size(); ++i) {
		m_vertexSizes.push_back(m_shapes[i].sizeVertices());
		m_indiceSizes.push_back(m_shapes[i].sizeIndices());
		m_indice_numbers.push_back(m_shapes[i].numIndices());
		m_bufferSize += m_vertexSizes.at(i) + m_indiceSizes.at(i);
		int x = 0;
	}

	glGenBuffers(1, &m_vertexBufferID);
	glBindBuffer(	m_target, m_vertexBufferID);
	glBufferData(	m_target, m_bufferSize, nullptr, GL_STATIC_DRAW);
	glBindBuffer(	m_target, m_vertexBufferID);

	std::size_t offset = 0;
	for (auto i = 0; i < m_shapes.size(); ++i) {
		glBufferSubData(m_target, offset, m_vertexSizes.at(i), m_shapes.at(i).vertices.data());
		offset += m_vertexSizes.at(i);
		glBufferSubData(m_target, offset, m_indiceSizes.at(i), &m_shapes.at(i).indices.front());
		offset += m_indiceSizes.at(i);
		m_arrayIDs.push_back(0);
		glGenVertexArrays(1, &m_arrayIDs.at(i));

	}

	std::size_t shapeOffset = 0;
	
	for (auto i = 0; i < m_shapes.size(); ++i) {
		glBindVertexArray(m_arrayIDs.at(i));
		offset = shapeOffset;
		glEnableVertexAttribArray(POSITION_ATTR);
		glVertexAttribPointer(POSITION_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offset);
		offset += sizeof(glm::tvec3<GLfloat>);
		glEnableVertexAttribArray(COLOR_ATTR);
		glVertexAttribPointer(COLOR_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offset);
		glEnableVertexAttribArray(NORMAL_ATTR);
		offset += sizeof(glm::tvec3<GLfloat>);
		glVertexAttribPointer(NORMAL_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offset);
		shapeOffset += m_shapes.at(i).sizeShape();
	}

	m_matrixBufferSize = sizeof(glm::mat4)*m_matrices.size();
	glGenBuffers(1, &m_viewMatrixBufferID);
	createMatrixBuffer(m_matrices.data(), m_matrixBufferSize, MODEL_ATTR, m_viewMatrixBufferID);
	glGenBuffers(1, &m_worldMatrixBufferID);
	createMatrixBuffer(m_matrices.data(), m_matrixBufferSize, WORLD_ATTR, m_worldMatrixBufferID);

}

void Buffer::createMatrixBuffer(const void* data, std::size_t size, std::uint32_t attribute, std::uint32_t vertexBufferID) {
	glBindBuffer(m_target, vertexBufferID);
	glBufferData(m_target, size, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(m_target, 0, size, (void*)data);


	GLsizeiptr offset = 0;
	
	for (auto j = 0; j < m_arrayIDs.size(); ++j) {
		glBindVertexArray(m_arrayIDs.at(j));
		glEnableVertexAttribArray(attribute);
		for (int i = 0; i < 4; ++i) {
			glEnableVertexAttribArray(attribute + i);
			glVertexAttribPointer(attribute + i, 4u, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)offset);
			glVertexAttribDivisor(attribute + i, 1);
			offset += sizeof(float) * 4;
		}
		offset += sizeof(glm::mat4)*(m_instances.at(j) - 1);
	}
}

void Buffer::drawGeo(const Camera& cam, const glm::mat4& projection) {
	GLsizeiptr shapeOffset = 0;
	GLsizeiptr matOffset = 0;
	std::vector<glm::mat4> MVPs;
	for (auto matrix : m_matrices) {
		MVPs.push_back(projection * cam.getWorldToViewMatrix() * matrix);
	}
	glBindBuffer(m_target, m_viewMatrixBufferID);
	glBufferSubData(m_target, 0, m_matrixBufferSize, &MVPs[0][0][0]);

	for (auto i = 0; i < m_shapes.size(); ++i) {
		shapeOffset += m_shapes.at(i).sizeVertices();
		glBindVertexArray(m_arrayIDs[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferID);
		glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)m_indice_numbers[i], GL_UNSIGNED_SHORT, (void*)shapeOffset, m_instances.at(i));

		shapeOffset += m_shapes.at(i).sizeIndices();
		matOffset += sizeof(glm::mat4);
	}
	

}