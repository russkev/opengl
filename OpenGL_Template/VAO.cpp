#pragma once
#include <iostream>
#include <cassert>
#include <GL/glew.h>

#include "Buffer.h"
#include "VAO.h"

// // CONSTRUCTOR //
VAO::VAO() :
	m_VAO_ID(0)
{
	std::cout << "VAO object created\n";
};

void VAO::GenerateVAO(const Buffer& inBuffer, std::size_t divisor, const member_info_type* begin, const member_info_type* end, std::uint32_t id_offset) {
	GenerateID();
	Bind();
	inBuffer.Bind();

	std::for_each(begin, end, [&](const member_info_type& mi)
	{
		for (auto i = 0u; i < mi.slot_occupancy; ++i)
		{
			glEnableVertexAttribArray(id_offset + mi.attribute_slot + i);
			glVertexAttribPointer(id_offset + mi.attribute_slot + i,mi.component_count, mi.gl_type_enum, GL_TRUE, mi.tuple_stride, mi.offset_from_start + i * mi.slot_size_in_bytes);
			glVertexAttribDivisor(id_offset + mi.attribute_slot + i, divisor);

		}
	});
}

void VAO::Bind() {
	glBindVertexArray(m_VAO_ID);
}

// // GENERATE ID
void VAO::GenerateID() {
	if (m_VAO_ID == 0) {
		glGenVertexArrays(1, &m_VAO_ID);
	}
	return;
}
