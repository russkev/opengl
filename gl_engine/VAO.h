#ifndef GL_ENGINE_VAO_H
#define GL_ENGINE_VAO_H

#include <vector>

#include "Buffer.h"

#include "GL_Tuple_Introspect.h"

namespace gl_engine
{
	struct VAO
	{
		// // CONSTRUCTOR
		VAO();

		// // GENERAL FUNCTIONS
		void GenerateID();
		void GenerateVAO(const Buffer& inBuffer, std::size_t divisor, const member_info_type* begin, const member_info_type* end, std::uint32_t id_offset = 0u);
		void Bind();

		// // GETTERS
		std::uint32_t VAO_ID() { return m_VAO_ID; }

	private:
		std::uint32_t					m_target;
		std::uint32_t					m_VAO_ID;
	};
} // namespace gl_engine
#endif