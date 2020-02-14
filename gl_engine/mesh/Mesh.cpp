#include "pch.h"
#include "Mesh.h"

namespace glen
{
	// // ----- CONSTRUCTORS ----- // //
	Mesh::Mesh() :
		m_vertices(vertices_type{}),
		m_indices(index_type{}),
		m_id(0u)
	{};

	Mesh::Mesh(const vertices_type s_vertices, const indices_type s_indices) :
		m_vertices(s_vertices),
		m_indices(s_indices)
	{};

	//// // ----- Move Constructor ----- // //
	//Mesh::Mesh(const Mesh&& other) :
	//	m_vertices(std::move(other.m_vertices)),
	//	m_indices(std::move(other.m_indices)),
	//	m_id(std::move(other.m_id))
	//{};

	//// // ----- Move Assign ----- // //
	//Mesh& Mesh::operator = (Mesh&& other)
	//{
	//	(*this).~Mesh();
	//	return *new (this) Mesh(std::move(other));
	//}

	// // ----- Addition Assign ----- // //
	Mesh& Mesh::operator += (Mesh& other)
	{
		auto currentNumVertices = (index_type)num_vertices();
		for (std::size_t i = 0u; i < other.num_indices(); ++i)
		{
			m_indices.push_back(other.m_indices.at(i) + currentNumVertices);
		}
		m_vertices.insert(m_vertices.end(), other.m_vertices.begin(), other.m_vertices.end());
		update_ids();
		return *this;
	}

	// // ----- APPEND ----- // //
	void Mesh::append_vertex(const Vertex vertex)
	{
		m_vertices.push_back(vertex);
	}

	void Mesh::append_triangle(const Vertex vertex_1, const Vertex vertex_2, const Vertex vertex_3)
	{
		m_vertices.push_back(vertex_1);
		m_vertices.push_back(vertex_2);
		m_vertices.push_back(vertex_3);
	}

	void Mesh::append_index(const GLushort pos)
	{
		m_indices.push_back(pos);
	}

	// ----- TRANSFORM ----- // //
	void Mesh::transform(glm::mat4 transformMatrix)
	{
		transform(m_vertices, transformMatrix);
	}

	void Mesh::transform(Mesh::vertices_type& inVertices, const glm::mat4 transformMatrix)
	{

		assert(inVertices.size() > 0);
		for (auto & vertex : inVertices)
		{
			// Transform the vertex
			auto position = transformMatrix * glm::vec4(vertex.position(), 1);
			auto normal = transformMatrix * glm::vec4(vertex.normal(), 1);
			auto tangent = transformMatrix * glm::vec4(vertex.tangent(), 1);
			auto bitangent = transformMatrix * glm::vec4(vertex.bitangent(), 1);

			// Set the new vertex attributes
			vertex.set_position(position);
			vertex.set_normal(normal);
			vertex.set_tangent(tangent);
			vertex.set_bitangent(bitangent);
		}
	}

	// // ------ UVs ----- // //
	void Mesh::scale_uvs(const GLfloat amount)
	{
		glm::mat2 scale_matrix{ amount, 0, 0, amount };
		for (Vertex & vertex : m_vertices)
		{
			glm::vec2 curent_uv = vertex.uv();
			glm::vec2 new_uv = scale_matrix * vertex.uv();
			vertex.set_uv(new_uv);
		}
	}

	// // ------ IDs ----- // //
	void Mesh::update_ids()
	{
		if (num_vertices() > 0)
		{
			for (auto & vertex : m_vertices)
			{
				vertex.set_id(m_id);
			}
		}
	}
	// // ------ INDICES ----- // //

	// Guess shared indices based on proximity
	void Mesh::make_indices_smooth()
	{
		make_indices_faceted();
		vertices_type newVertices;
		indices_type newIndices;
		std::map<Vertex, index_type> vertexMap;

		for (int i = 0; i < m_vertices.size(); i++)
		{
			int existingIndex = find_similar_vertex(i, vertexMap);
			if (existingIndex >= 0)
			{
				newIndices.push_back((index_type)existingIndex);
			}
			else
			{
				newVertices.push_back(m_vertices.at(i));
				index_type newIndex = (index_type)newVertices.size() - 1;
				newIndices.push_back(newIndex);
				vertexMap[m_vertices.at(i)] = newIndex;
			}
		}
		std::swap(newVertices, m_vertices);
		std::swap(newIndices, m_indices);
	}

	void Mesh::make_indices_faceted()
	{
		for (auto i = 0; i < m_vertices.size(); i++)
		{
			m_indices.push_back((index_type)i);
		}
	}

	int Mesh::find_similar_vertex(const index_type s_currentVertIndex, const std::map<Vertex, index_type>& s_vertexMap)
	{
		auto iterator = s_vertexMap.find(m_vertices.at(s_currentVertIndex));
		if (iterator == s_vertexMap.end())
		{
			return -1;
		}
		else
		{
			return iterator->second;
		}
	}

	void Mesh::set_index(std::size_t loc, const index_type& data)
	{
		assert(num_indices() >= loc);
		m_indices.at(loc) = data;
	}

	// // ----- TANGENTS AND BITANGENTS ----- // //
	void Mesh::make_tangents()
	{
		assert(m_vertices.size() > 0);
		assert(m_indices.size() > 0);
		std::vector<index_type> existingIndices;

		// Cycle through sets of three indices
		for (auto i = 0; i < m_indices.size(); i += 3)
		{
			// Get vertices
			const glm::vec3& v0 = m_vertices.at(m_indices.at(i + 0)).position();
			const glm::vec3& v1 = m_vertices.at(m_indices.at(i + 1)).position();
			const glm::vec3& v2 = m_vertices.at(m_indices.at(i + 2)).position();

			// Get UVs
			const glm::vec2& uv0 = m_vertices.at(m_indices.at(i + 0)).uv();
			const glm::vec2& uv1 = m_vertices.at(m_indices.at(i + 1)).uv();
			const glm::vec2& uv2 = m_vertices.at(m_indices.at(i + 2)).uv();

			// Edges of the triangle, position delta
			glm::vec3 deltaPos1 = v1 - v0;
			glm::vec3 deltaPos2 = v2 - v0;

			// UV delta
			glm::vec2 deltaUV1 = uv1 - uv0;
			glm::vec2 deltaUV2 = uv2 - uv0;

			// Calculate tangent and bitangent
			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			glm::vec3 newTangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
			glm::vec3 newBitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

			// If tangent and bitangents exist, calculate the average of the current and the new tangents, otherwise just set the new tangent
			for (auto j = 0; j < 3; ++j) {
				if (std::find(existingIndices.begin(), existingIndices.end(), m_indices.at(i + j)) != existingIndices.end()) {

					// Get existing tangents
					glm::vec3 existingTangent = m_vertices.at(m_indices.at(i + j)).tangent();
					glm::vec3 existingBitangent = m_vertices.at(m_indices.at(i + j)).bitangent();

					// Calculate and set the average of the tangents. Good for them to be non-normalised here.
					m_vertices.at(m_indices.at(i + j)).set_tangent((existingTangent + newTangent) / 2.0f);
					m_vertices.at(m_indices.at(i + j)).set_bitangent((existingBitangent + newBitangent) / 2.0f);

				}
				else {
					// Set the new tangents
					m_vertices.at(m_indices.at(i + j)).set_tangent(newTangent);
					m_vertices.at(m_indices.at(i + j)).set_bitangent(newBitangent);
				}

			}

			// Register used indices
			for (auto j = 0; j < 3; ++j) {
				existingIndices.push_back(m_indices.at(i + j));
			}
		}
	}

	void Mesh::set_id(GLuint id)
	{
		if (num_vertices() > 0)
		{
			for (auto & vertex : m_vertices)
			{
				vertex.set_id(id);
			}
		}

	}

	// // ----- GETTERS ----- // //
	Vertex* Mesh::get_vertex(std::size_t pos)
	{
		assert(num_vertices() >= pos);
		return &m_vertices.at(pos);
	}

	Mesh::index_type Mesh::get_index(std::size_t pos)
	{
		assert(num_indices() >= pos);
		return m_indices.at(pos);
	}

	// // ----- SETTERS ----- // //
	void Mesh::set_vertex(std::size_t pos, Vertex& vertex)
	{
		assert(num_vertices() >= pos);

		m_vertices.at(pos) = std::move(vertex);
	}

	void Mesh::set_vertex_position(GLuint pos, glm::vec3 position)
	{
		assert(num_vertices() >= pos);

		m_vertices.at(pos).set_position(position);
	}


}