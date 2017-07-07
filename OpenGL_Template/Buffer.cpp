#include <GL/glew.h>

#include "Buffer.h"
#include "Vertex.h"

static constexpr auto POSITION_ATTR = 0u;
static constexpr auto COLOR_ATTR = 1u;
static constexpr auto NORMAL_ATTR = 2u;

Buffer::Buffer(std::uint32_t target_, std::size_t initial_length_):
	m_target(target_),
	m_length(initial_length_)
{
}


void Buffer::createGeoBuffer(const void* data, std::size_t size)
{
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(m_target, m_bufferID);
	glBufferData(m_target, size, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(m_target, 0, size, (void*)data);

	std::uint32_t vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	glEnableVertexAttribArray(POSITION_ATTR);
	glVertexAttribPointer(POSITION_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(COLOR_ATTR);
	glVertexAttribPointer(COLOR_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::tvec3<GLfloat>));
	glEnableVertexAttribArray(NORMAL_ATTR);
	glVertexAttribPointer(NORMAL_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::tvec3<GLfloat>) * 2));
}

void Buffer::createMatrixBuffer(const void* data, std::size_t size, std::uint32_t attribute) {
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(m_target, m_bufferID);
	glBufferData(m_target, size, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(m_target, 0, size, (void*)data);

	std::uint32_t vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	glEnableVertexAttribArray(attribute);


}

void Buffer::createBuffer(const void* data, std::size_t size) {
	return;
}

void Buffer::createVAOs(const void* data, std::vector<std::uint32_t> attributes, std::uint32_t step, std::uint32_t initialOffset) {
	return;
}