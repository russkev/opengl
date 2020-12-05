#include "pch.h"
#include "VAO.h"

#include "Buffer.h"

namespace glen
{
	// // ----- CONSTRUCTOR ----- // //
	VAO::VAO() :
		m_VAO_ID(0)
	{};

	VAO::VAO(VAO&& other) noexcept:
		m_VAO_ID{ std::exchange(other.m_VAO_ID, 0)}
	{}

	VAO& VAO::operator = (VAO&& other) noexcept
	{
		(this)->~VAO();
		return *new (this) VAO(std::move(other));
	}

	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &m_VAO_ID);
	}

	// // ----- GENERAL METHODS ----- // //
	void VAO::generate_VAO(const Buffer& inBuffer, std::size_t divisor, const member_info_type* begin, const member_info_type* end, std::uint32_t id_offset) {
		generate_id();
		bind();
		inBuffer.bind();

		std::for_each(begin, end, [&](const member_info_type& member_info)
		{
			for (auto i = 0u; i < member_info.slot_occupancy; ++i)
			{
				glEnableVertexAttribArray(id_offset + member_info.attribute_slot + i);
				if (member_info.gl_type_enum == GL_FLOAT || member_info.gl_type_enum == GL_DOUBLE)
				{
					glVertexAttribPointer(id_offset + member_info.attribute_slot + i, member_info.component_count, member_info.gl_type_enum, GL_TRUE, member_info.tuple_stride, member_info.offset_from_start + i * member_info.slot_size_in_bytes);
				}
				else
				{
					glVertexAttribIPointer(id_offset + member_info.attribute_slot + i, member_info.component_count, member_info.gl_type_enum, member_info.tuple_stride, member_info.offset_from_start + i * member_info.slot_size_in_bytes);
				}
				glVertexAttribDivisor(id_offset + member_info.attribute_slot + i, (GLuint)divisor);

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
}