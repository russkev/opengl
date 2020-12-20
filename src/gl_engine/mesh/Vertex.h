#ifndef GL_ENGINE_MESH_VERTEX_H
#define GL_ENGINE_MESH_VERTEX_H

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace glen
{

	// // ----- ENUMERATIONS ----- // //
	enum vertexAttr { position_loc = 0, color_loc = 1, normal_loc = 2, uv_loc = 3, id_loc = 4, tangent_loc = 5, bitangent_loc = 6 };

	/*!
	* 
	* Individual vertex.
	* Basic building block of a mesh.
	* Contains various attributes which are useful for sending to the shader
	* A vertex is structured in the following way:
	* 
	* @rst
	* 
	* +----------------+------+-----------------------+
	* | Tuple location | Type | Name				  |
	* +================+======+=======================+
	* | 0			   | Vec3 | Position coordinates  |
	* +----------------+------+-----------------------+
	* | 1			   | Vec3 | Color				  |
	* +----------------+------+-----------------------+
	* | 2			   | Vec3 | Normals				  |
	* +----------------+------+-----------------------+
	* | 3			   | Vec2 | UV Coordinates		  |
	* +----------------+------+-----------------------+
	* | 4			   | Int  | ID					  |
	* +----------------+------+-----------------------+
	* | 5			   | Vec3 | Tangent				  |
	* +----------------+------+-----------------------+
	* | 6			   | Vec3 | Bitangent			  |
	* +----------------+------+-----------------------+
	* 
	* @endrst
	*/
	struct Vertex
	{
		// @cond

		// // ----- CONSTANTS ----- // //
		static constexpr float DISTANCE_THRESHOLD = 0.01f;

		// // ----- TYPE DEFINITIONS ----- // //
		// --- Don't forget to update the gl_introspect_tuple call when changing this --- //
		//                   Position,   Color,    Normal,      UV,     ID,    Tangent,  Bitangent    //
		typedef std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint, glm::vec3, glm::vec3>	vertexType;

		// @endcond

		// // ----- CONSTRUCTORS ----- // //

		/*!
		 * @brief Constructor.
		*/
		Vertex();

		/*!
		 * @brief Constructor.
		*/
		Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv);

		/*!
		 * @brief Constructor.
		*/
		Vertex(glm::vec3 position, glm::vec3 color, glm::vec3 normal, glm::vec2 uv);

		/*!
		 * @brief Constructor.
		*/
		Vertex(glm::vec3 position, glm::vec3 color, glm::vec3 normal, glm::vec2 uv, GLint id, glm::vec3 tangent, glm::vec3 bitangent);

		// // ----- OPERATOR OVERLOADS ----- // //
		
		/*!
		 * @return True if this vertex has a higher value in memory than `other`.
		*/
		bool operator < (const Vertex& other) const;

		// // ----- GETTERS ----- // //

		/*!
		 * @return Tuple containing all vertex information.
		*/
		const vertexType& vertex_tuple() const;

		/*!
		 * @return Position coordinate of vertex.
		*/
		const glm::vec3& position() const;

		/*!
		 * @return Color r, g, b vector of vertex.
		*/
		const glm::vec3& color() const;

		/*!
		 * @return Normal vector of vertex.
		*/
		const glm::vec3& normal() const;

		/*!
		 * @return UV coordinates of vertex.
		*/
		const glm::vec2& uv() const;

		/*!
		 * @return Unique ID of vertex.
		*/
		const GLint& id() const;

		/*!
		 * @return Tangent vector of vertex.
		*/
		const glm::vec3& tangent() const;

		/*!
		 * @return Bitangent vector of vertex.
		*/
		const glm::vec3& bitangent() const;

		// // ----- SETTERS ----- // //

		/*!
		 * @brief Set the position coordinate of vertex.
		*/
		void set_position(const glm::vec3& position);

		/*!
		 * @brief Set the r, g, b color vector of vertex.
		*/
		void set_color(const glm::vec3& color);

		/*!
		 * @brief Set the normal vector of the vertex.
		*/
		void set_normal(const glm::vec3& normal);

		/*!
		 * @brief Set the UV coordinates of the vertex.
		*/
		void set_uv(const glm::vec2& uv);

		/*!
		 * @brief Set the ID of the vertex (should be unique to the corresponding mesh).
		 * @param id 
		*/
		void set_id(const GLint id);

		/*!
		 * @brief Set the tangent vector of the vertex
		*/
		void set_tangent(const glm::vec3& tangent);

		/*!
		 * @brief Set the bitangent vector of the vertex
		*/
		void set_bitangent(const glm::vec3& bitangent);

	private:
		// // ----- MEMBER VARIABLES ----- // //
		vertexType m_vertex;
	};
}
#endif
