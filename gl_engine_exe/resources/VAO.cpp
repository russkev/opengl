#pragma once
#include <iostream>
#include <cassert>
#include <GL/glew.h>

#include "Buffer.h"
#include "VAO.h"

namespace gl_engine_exe
{
	// // ----- CONSTRUCTOR ----- // //
	VAO::VAO() :
		m_VAO_ID(0)
	{};

	// // ----- GENERAL METHODS ----- // //
	void VAO::generate_VAO(const Buffer& inBuffer, std::size_t divisor, const member_info_type* begin, const member_info_type* end, std::uint32_t id_offset) {
		generate_id();
		bind();
		inBuffer.bind();

		std::for_each(begin, end, [&](const member_info_type& mi)
		{
			for (auto i = 0u; i < mi.slot_occupancy; ++i)
			{
				glEnableVertexAttribArray(id_offset + mi.attribute_slot + i);
				if (mi.gl_type_enum == GL_FLOAT || mi.gl_type_enum == GL_DOUBLE)
				{
					glVertexAttribPointer(id_offset + mi.attribute_slot + i, mi.component_count, mi.gl_type_enum, GL_TRUE, mi.tuple_stride, mi.offset_from_start + i * mi.slot_size_in_bytes);
				}
				else
				{
					glVertexAttribIPointer(id_offset + mi.attribute_slot + i, mi.component_count, mi.gl_type_enum, mi.tuple_stride, mi.offset_from_start + i * mi.slot_size_in_bytes);
				}
				glVertexAttribDivisor(id_offset + mi.attribute_slot + i, (GLuint)divisor);

			}
		});
	}

	void VAO::bind() {
		glBindVertexArray(m_VAO_ID);
	}

	void VAO::generate_id() {
		if (m_VAO_ID == 0) {
			glGenVertexArrays(1, &m_VAO_ID);
		}
		return;
	}
} // namespace gl_engine
