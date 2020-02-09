#ifndef GL_ENGINE_VAO_H
#define GL_ENGINE_VAO_H

#include <vector>

#include "Buffer.h"
#include "GL_Tuple_Introspect.h"

namespace glen
{
	/*
	
		Helper class for generating VAOs
	
	*/
	struct VAO
	{
		// // ----- CONSTRUCTOR ----- // //
	public:
		VAO();
		~VAO();

		// // ----- GENERAL METHODS ----- // //
		void generate_id();
		void generate_VAO(const Buffer& inBuffer, std::size_t divisor, const member_info_type* begin, const member_info_type* end, std::uint32_t id_offset = 0u);
		void bind();

		// // ----- GETTERS ----- // //
		std::uint32_t VAO_id() { return m_VAO_ID; }

		// // ----- MEMBER VARIABLES ----- // //
	private:
		std::uint32_t					m_target;
		std::uint32_t					m_VAO_ID;
	};
}
#endif