#ifndef GL_ENGINE_MESH_VERTEX_H
#define GL_ENGINE_MESH_VERTEX_H

#include <tuple>

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace glen
{
	/*

	Individual vertex.
	Contains various attributes which are useful for sending to the shader

	*/
	// // ----- ENUMERATIONS ----- // //
	enum vertexAttr { position_loc = 0, color_loc = 1, normal_loc = 2, uv_loc = 3, id_loc = 4, tangent_loc = 5, bitangent_loc = 6 };

	struct Vertex
	{
		// // ----- CONSTANTS ----- // //
		static constexpr float DISTANCE_THRESHOLD = 0.01f;

		// // ----- TYPE DEFINITIONS ----- // //
		// --- Don't forget to update the gl_introspect_tuple call when changing this --- //
		//                   Position,   Colour,    Normal,      UV,     ID,    Tangent,  Bitangent    //
		typedef std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint, glm::vec3, glm::vec3>	vertexType;

		// // ----- CONSTRUCTORS ----- // //
		Vertex();
		Vertex(glm::vec3, glm::vec3, glm::vec2);
		Vertex(glm::vec3, glm::vec3, glm::vec3, glm::vec2);
		Vertex(glm::vec3 position, glm::vec3 color, glm::vec3 normal, glm::vec2 uv, GLint id, glm::vec3 tangent, glm::vec3 bitangent);

		// // ----- OPERATOR OVERLOADS ----- // //
		bool operator < (const Vertex&) const;

		// // ----- GETTERS ----- // //
		const vertexType& vertex_tuple() const;
		const glm::vec3& position() const;
		const glm::vec3& color() const;
		const glm::vec3& normal() const;
		const glm::vec2& uv() const;
		const GLint& id() const;
		const glm::vec3& tangent() const;
		const glm::vec3& bitangent() const;

		// // ----- SETTERS ----- // //
		void set_position(const glm::vec3& position);
		void set_color(const glm::vec3& color);
		void set_normal(const glm::vec3& normal);
		void set_uv(const glm::vec2& uv);
		void set_id(const GLint id);
		void set_tangent(const glm::vec3& tangent);
		void set_bitangent(const glm::vec3& bitangent);

	private:
		// // ----- MEMBER VARIABLES ----- // //
		vertexType m_vertex;
	};
}
#endif
