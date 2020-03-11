#ifndef GL_ENGINE_MESH_MESH_H
#define GL_ENGINE_MESH_MESH_H

#include <vector>
#include <cassert>
#include <string>
#include <map>
#include <cstdlib>

#include <GL\glew.h>
#include <glm/glm.hpp>

#include "utils/VectorUtils.h"
#include "Vertex.h"

namespace glen
{
	/*

	Raw mesh type.
	Contains verticies, vertex properties and indicies.

	*/
	struct Mesh
	{
	public:

		// // ----- TYPE DEFINITIONS ----- // //
		typedef std::vector<Vertex>						vertices_type;
		typedef GLushort								index_type;
		typedef std::vector<index_type>					indices_type;
		typedef std::vector<Vertex>::const_iterator		vrt_iterator;
		typedef std::vector<GLushort>::const_iterator	ind_iterator;

		// // ----- ATTRIBUTE ENUMERATION ----- // //
		//enum attr { position = 0, color = 1, normal = 2, uv = 3, id = 4, tangent = 5, bitangent = 6 };

		// // ----- BIG 6 ----- // //
	public:
		Mesh();
		Mesh(const vertices_type vertices, const indices_type indices);
		Mesh(const Mesh&) = delete;
		Mesh(Mesh&& other) = default;
		Mesh& operator = (const Mesh&) = delete;
		Mesh& operator = (Mesh&& other) = default;
		~Mesh() {};


		// // ----- OPERATOR OVERLOADS ----- // //
		Mesh& operator += (Mesh& other);

		// // ----- APPEND ----- // //
		void append_vertex(const Vertex shape);
		void append_triangle(const Vertex vertex_1, const Vertex vertex_2, const Vertex vertex_3);
		void append_index(const GLushort index);

		// // ----- TRANSFORM ----- // //
		void transform(glm::mat4 transformMatrix);
		void transform(vertices_type& inVertices, const glm::mat4 transformMatrix);

		// // ----- UVs ----- // //
		void scale_uvs(const GLfloat amount);

		// // ----- IDs ----- // //
		void update_ids();

		// // ------INDICES ----- // //
		void make_indices_smooth();
		void make_indices_faceted();
		int find_similar_vertex(const index_type, const std::map<Vertex, index_type>&);

		// // ----- TANGENTS AND BITANGENTS ----- // //
		void make_tangents();

		// // ----- GETTERS ----- // //
		Vertex* get_vertex(std::size_t pos);
		template <std::size_t attr>
		glm::vec3 get_vertex(const std::size_t pos)
		{
			assert(num_vertices() > 0);
			return std::get<attr>(m_vertices.at(pos));
		}
		index_type get_index(std::size_t i);

		std::size_t num_indices() { return m_indices.size(); }
		std::size_t num_vertices() { return m_vertices.size(); }
		vertices_type vertices() { return m_vertices; }
		const indices_type& indices() const { return m_indices; }
		const GLuint id() const;

		// // ----- SIZE GETTERS----- // //
		GLsizeiptr size_vertices() { return m_vertices.size() * sizeof(Vertex); }
		GLsizeiptr size_indices() { return m_indices.size() * sizeof(index_type); }
		GLsizeiptr size_shape() { return size_vertices() + size_indices(); }

		// // ----- SETTERS ----- // //
		void set_vertex(std::size_t pos, Vertex& data);
		template <std::size_t attr>
		void set_vertex(std::size_t pos, const glm::vec3& data)
		{
			assert(num_vertices() >= pos);
			m_vertices.at(pos).position() = data;
		}
		void set_vertex_position(GLuint pos, glm::vec3 position);
		void set_index(std::size_t pos, const index_type& data);
		void set_id(GLuint s_id);


		// // ----- ITERATORS ----- // //
		vrt_iterator vert_begin() { return m_vertices.begin(); }
		vrt_iterator vert_end() { return m_vertices.end(); }
		ind_iterator indx_begin() { return m_indices.begin(); }
		ind_iterator indx_end() { return m_indices.end(); }

		// // ----- MEMBER VARIABLES----- // //
	private:
		vertices_type	m_vertices;
		indices_type	m_indices;
		GLuint			m_id;
	};
}
#endif