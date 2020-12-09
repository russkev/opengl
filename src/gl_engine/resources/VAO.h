#ifndef GL_ENGINE_VAO_H
#define GL_ENGINE_VAO_H

#include "Buffer.h"
#include "GL_Tuple_Introspect.h"

namespace glen
{
	/*!
	
		Helper class for generating VAOs (Vertex Array Objects)
	
	*/
	struct VAO
	{
		// // ----- CONSTRUCTOR ----- // //
	public:
		/*!
		 * @brief Constructor
		*/
		VAO();

		/*!
		 * @brief Use move constructor instead of copy constructor.
		*/
		VAO(const VAO& other) = delete;

		/*!
		 * @brief Move constructor.
		*/
		VAO(VAO&& other) noexcept;

		/*!
		 * @brief Use move assign instead of copy assign.
		*/
		VAO& operator = (const VAO& other) = delete;

		/*!
		 * @brief Move assign.
		*/
		VAO& operator = (VAO&& other) noexcept;
		~VAO();

		
		// // ----- GENERAL METHODS ----- // //
		
		/*!
		 * @brief Generate the Vertex Array Object
		 * @param inBuffer The `Buffer` object to use
		 * @param divisor Useful for rendering instances. This defines how the attributes advance when instances are being rendered.
		 * Usually this is set to 0.
		 * @param begin The momory location of the start of the data.
		 * @param end The momory location of the end of the data.
		 * @param id_offset The start shader layout location that should be associated with this VAO.
		*/
		void generate_VAO(const Buffer& inBuffer, std::size_t divisor, const member_info_type* begin, const member_info_type* end, std::uint32_t id_offset = 0u);
		
		/*!
		 * @brief Generate an OpenGL ID for this VAO.
		*/
		void generate_id();

		/*!
		 * @brief Make this VAO 'active' with regards to OpenGL.
		*/
		void bind();

		
		// // ----- GETTERS ----- //

		/*!
		 * @return The OpenGL VAO ID associated with this object.
		*/
		std::uint32_t id() { return m_VAO_ID; }

		// // ----- MEMBER VARIABLES ----- // //
	private:
		std::uint32_t					m_VAO_ID;
	};
}
#endif