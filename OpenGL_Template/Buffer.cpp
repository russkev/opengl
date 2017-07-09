#include <GL/glew.h>

#include "Buffer.h"
#include "Vertex.h"

static constexpr auto POSITION_ATTR = 0u;
static constexpr auto COLOR_ATTR = 1u;
static constexpr auto NORMAL_ATTR = 2u;
static constexpr auto MODEL_ATTR = 4u;
static constexpr auto WORLD_ATTR = 8u;

Buffer::Buffer(std::uint32_t target_, std::size_t initial_length_) :
	m_target(target_),
	m_length(initial_length_),
	m_vertexBufferID(0)
	//m_arrayBufferID(0)
{
}


void Buffer::createGeoBuffer(const void* data, std::size_t size)
{
	glGenBuffers(1, &m_vertexBufferID);
	glBindBuffer(m_target, m_vertexBufferID);
	glBufferData(m_target, size, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(m_target, 0, size, (void*)data);

	glGenVertexArrays(1, &m_arrayID);
	glBindVertexArray(m_arrayID);
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

void Buffer::createMatrixBuffer(const void* data, std::size_t size, std::uint32_t attribute, std::uint32_t vertexBufferID) {
	//m_arrayID = vertexArrayID;
	//glGenBuffers(1, &m_vertexBufferID);
	glBindBuffer(m_target, vertexBufferID);
	glBufferData(m_target, size, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(m_target, 0, size, (void*)data);

	glBindVertexArray(m_arrayID);
	glEnableVertexAttribArray(attribute);
	for (int i = 0; i < 4; ++i) {
		glEnableVertexAttribArray(attribute + i);
		glVertexAttribPointer(attribute + i, 4u, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * (i * 4)));
		glVertexAttribDivisor(attribute + i, 1);
	}


}

//void Buffer::createVAOs(const void* data, std::vector<std::uint32_t> attributes, std::uint32_t step, std::uint32_t initialOffset) {
//	return;
//}

std::uint32_t Buffer::getBufferID() {
	return m_vertexBufferID;
}

std::uint32_t Buffer::getArrayID() {
	return m_arrayID;
}

std::uint32_t Buffer::getViewMatrixBufferID() {
	return m_viewMatrixBufferID;
}

std::uint32_t Buffer::getWorldMatrixBufferID() {
	return m_worldMatrixBufferID;
}