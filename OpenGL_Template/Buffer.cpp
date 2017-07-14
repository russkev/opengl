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

Buffer::Buffer(std::uint32_t target_, std::size_t initial_length_) :
	m_target(target_),
	m_length(initial_length_),
	m_vertexBufferID(0),
	m_bufferSize(0)
{
}


void Buffer::createGeoBuffer(const std::vector<ShapeData>& shapes)
{
	m_shapes = shapes;
	for (auto i = 0; i < m_shapes.size(); ++i) {
		m_vertexSizes.push_back(m_shapes[i].sizeVertices());
		m_indiceSizes.push_back(m_shapes[i].sizeIndices());
		m_indice_numbers.push_back(m_shapes[i].numIndices());
		m_bufferSize += m_vertexSizes.at(i) + m_indiceSizes.at(i);
		int x = 0;// 45396
	}

	//m_bufferSize += 100000;
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

	for (auto i = 0; i < m_shapes.size(); ++i){
		glBindVertexArray(m_arrayIDs.at(i));
		glEnableVertexAttribArray(POSITION_ATTR);
		glVertexAttribPointer(POSITION_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(COLOR_ATTR);
		glVertexAttribPointer(COLOR_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::tvec3<GLfloat>));
		glEnableVertexAttribArray(NORMAL_ATTR);
		glVertexAttribPointer(NORMAL_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::tvec3<GLfloat>) * 2));
		const glm::mat4 identityMatrix = glm::mat4(1.0);
		glGenBuffers(1, &m_viewMatrixBufferID);
		createMatrixBuffer(&identityMatrix, sizeof(glm::mat4), MODEL_ATTR, m_viewMatrixBufferID);
		glGenBuffers(1, &m_worldMatrixBufferID);
		createMatrixBuffer(&identityMatrix, sizeof(glm::mat4), WORLD_ATTR, m_worldMatrixBufferID);
	}
}

void Buffer::createMatrixBuffer(const void* data, std::size_t size, std::uint32_t attribute, std::uint32_t vertexBufferID) {
	glBindBuffer(m_target, vertexBufferID);
	glBufferData(m_target, size, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(m_target, 0, size, (void*)data);

	for (auto arrayID : m_arrayIDs) {
		glBindVertexArray(arrayID);
		glEnableVertexAttribArray(attribute);
		for (int i = 0; i < 4; ++i) {
			glEnableVertexAttribArray(attribute + i);
			glVertexAttribPointer(attribute + i, 4u, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * (i * 4)));
			glVertexAttribDivisor(attribute + i, 1);
		}
	}
}

void Buffer::drawGeo(const Camera& cam, const glm::mat4& projection) {
	GLsizeiptr offset = 0;
	for (auto i = 0; i < m_shapes.size(); ++i) {
		offset += m_shapes.at(i).sizeVertices();
		glBindVertexArray(m_arrayIDs[i]);
		glBindBuffer(m_target, m_viewMatrixBufferID);
		glm::mat4 MVP = projection * cam.getWorldToViewMatrix();
		glBufferSubData(m_target, 0, sizeof(glm::mat4), &MVP[0][0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferID);
		glDrawElementsInstanced(GL_TRIANGLES, m_indice_numbers[i], GL_UNSIGNED_SHORT, (void*)offset/*m_vertexSizes[i]*/, 1);
		if (i < m_shapes.size() - 1) {
			//offset += 8;
			//offset += m_indiceSizes[i];
			offset += m_shapes.at(i).sizeIndices();
		}
	}
	

}


std::uint32_t Buffer::getBufferID() {
	return m_vertexBufferID;
}

std::uint32_t Buffer::getViewMatrixBufferID() {
	return m_viewMatrixBufferID;
}

std::uint32_t Buffer::getWorldMatrixBufferID() {
	return m_worldMatrixBufferID;
}