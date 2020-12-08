#ifndef GL_ENGINE_MESH_MESH_H
#define GL_ENGINE_MESH_MESH_H

#include "utils/VectorUtils.h"
#include "Vertex.h"

namespace glen
{
	/*!

		Raw mesh type.
	
		Vertices define the points that make up the mesh.
		Indices should be provided in sets of three and define to connect the points with triangular faces.

	*/
	struct Mesh
	{
	public:

		// // ----- TYPE DEFINITIONS ----- // //
		typedef std::vector<Vertex>							vertices_type;
		typedef GLushort									index_type;
		typedef std::vector<index_type>						indices_type;
		typedef std::vector<Vertex>::const_iterator			vrt_iterator;
		typedef std::vector<GLushort>::const_iterator		ind_iterator;

		// // ----- ATTRIBUTE ENUMERATION ----- // //
		//enum attr { position = 0, color = 1, normal = 2, uv = 3, id = 4, tangent = 5, bitangent = 6 };

		// // ----- BIG 6 ----- // //
	public:
		/*!
		 * @brief Constructor
		*/
		Mesh();

		/*!
		 * @brief Constructor
		 * @param vertices The vertices that will make up the mesh
		 * @param indices The indices that will be used to specify the triangles of the mesh
		*/
		Mesh(std::shared_ptr<vertices_type> vertices, std::shared_ptr<indices_type> indices);

		/*!
		 * @brief Use move constructor instead of copy constructor
		*/
		Mesh(const Mesh&) = delete;

		/*!
		 * @brief Move constructor
		*/
		Mesh(Mesh&& other) = default;

		/*!
		 * @brief Use move assign instead of copy assign
		*/
		Mesh& operator = (const Mesh&) = delete;

		/*!
		 * @brief Move assign
		*/
		Mesh& operator = (Mesh&& other) = default;

		/*!
		* @brief destructor
		*/
		~Mesh() {};


		// // ----- OPERATOR OVERLOADS ----- // //

		/*!
		 * @brief Combine this mesh with another mesh
		 * @param other The other mesh
		 * @return The combined mesh
		*/
		Mesh& operator += (Mesh& other);

		
		// // ----- APPEND ----- // //

		/*!
		 * @param vertex The vertex to add 
		*/
		void append_vertex(const Vertex vertex);

		/*!
		 * @brief Add three vertices. Indices will need to be set up seperately
		*/
		void append_triangle(const Vertex vertex_1, const Vertex vertex_2, const Vertex vertex_3);

		/*!
		 * @brief Add a single index to a specify an existing vertex as part of a triangle.
		 * 
		 * This should be called in groups of three in order to add whole triangles.
		 * @param index Vertex index to add
		*/
		void append_index(const GLushort index);

		// // ----- TRANSFORM ----- // //

		/*!
		 * @brief Move all vertices in mesh according to `transformMatrix`.
		*/
		void transform(glm::mat4 transformMatrix);

	private:
		void transform(std::shared_ptr<vertices_type> inVertices, const glm::mat4 transformMatrix);

		// // ----- UVs ----- // //
	public:

		/*!
		 * @brief Scale all UVs evenly. Useful for making use of tiled textures.
		*/
		void scale_uvs(const GLfloat amount);

		// // ----- TRIANGLES ----- // //
		
		/*!
		 * @brief Reverse the facing direction of all triangles in the mesh by reversing the winding direction.
		*/
		void reverse_triangles();

		// // ----- IDs ----- // //
	private:
		void update_ids();

		// // ------INDICES ----- // //
	public:
		/*!
		 * @brief Interpolate normals so faces appear smooth.
		*/
		void make_indices_smooth();

		/*!
		 * @brief Don't interpolate normals. All faces will appear flat.
		*/
		void make_indices_faceted();
	private:
		int find_similar_vertex(const index_type, const std::map<Vertex, index_type>&);

		// // ----- TANGENTS AND BITANGENTS ----- // //
	public:
		/*!
		 * @brief Automatically make all tangents and bitangents for mesh
		 * 
		 * Properly set up tangents are required proper display.
		*/
		void make_tangents();

		// // ----- GETTERS ----- // //

		/*!
		 * @return The vertex at index: `pos`.
		*/
		Vertex* get_vertex(std::size_t pos);

		/*!
		 * @return Vertex index located at `pos` in index array.
		*/
		index_type get_index(std::size_t pos);

		/*!
		 * @return The number of indices.
		*/
		std::size_t num_indices() { return m_indices->size(); }

		/*!
		 * @return The number of vertices.
		*/
		std::size_t num_vertices() { return m_vertices->size(); }

		/*!
		 * @return Pointer to all vertices in the mesh.
		*/
		std::shared_ptr<vertices_type> vertices() { return m_vertices; }

		/*!
		 * @return Pointer to all indices in the mesh.
		*/
		const std::shared_ptr<indices_type>& indices() const { return m_indices; }

		/*!
		 * @return Unique ID for this mesh.
		*/
		const GLuint id() const;

		// // ----- SIZE GETTERS----- // //
		
		/*!
		 * @return The size in bytes of all vertices in the mesh.
		*/
		GLsizeiptr size_vertices() { return m_vertices->size() * sizeof(Vertex); }

		/*!
		 * @return The size in bytes of all indices in the mesh.
		*/
		GLsizeiptr size_indices() { return m_indices->size() * sizeof(index_type); }

		/*!
		 * @return The size in bytes of all indices and vertices in the mesh.
		*/
		GLsizeiptr size_shape() { return size_vertices() + size_indices(); }

		// // ----- SETTERS ----- // //

		/*!
		 * @brief Set vertex at index `pos` to be `data`.
		*/
		void set_vertex(std::size_t pos, Vertex& data);

		/*!
		 * @brief Set vertex's coordinates at index `pos` to `position`.
		*/
		void set_vertex_position(GLuint pos, glm::vec3 position);

		/*!
		 * @brief Set index at index array position `pos` to be `data`.
		*/
		void set_index(std::size_t pos, const index_type& data);
		void set_id(GLuint s_id);


		// // ----- ITERATORS ----- // //
		
		/*!
		 * @return Iterator to start of vertices.
		*/
		vrt_iterator vert_begin() { return m_vertices->begin(); }

		/*!
		 * @return Iterater to end of veritces.
		*/
		vrt_iterator vert_end() { return m_vertices->end(); }

		/*!
		 * @return Iterator to start of indices.
		*/
		ind_iterator indx_begin() { return m_indices->begin(); }

		/*!
		 * @return Iterator to end of indies.
		*/
		ind_iterator indx_end() { return m_indices->end(); }

		// // ----- MEMBER VARIABLES----- // //
	private:
		std::shared_ptr<vertices_type>	m_vertices;
		std::shared_ptr<indices_type>	m_indices;
		GLuint							m_id;
	};
}
#endif