#include "pch.h"
#include "OBJ.h"

#include <stdexcept>
#include <cassert>
#include <stdio.h>

#include <glm/matrix.hpp>

#include "Vertex.h"

namespace glen
{
	// // ------ CONSTANTS ----- // //
	const glm::vec3 OBJ_Loader::DEFAULT_COLOUR = { 0.5, 0.5, 0.5 };

	// // ------ MAIN ----- // //
	Mesh OBJ_Loader::load_obj(const char *path)
	{
		// Create temp variables
		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< glm::vec3 > tempVertices;
		std::vector< glm::vec2 > tempUVs;
		std::vector< glm::vec3 > tempNormals;
		Mesh new_shape;
		errno_t error;
		FILE *file;

		// Attempt to open file
		//FILE * file = fopen(path, "r");
		error = fopen_s(&file, path, "r");
		if (error != 0)
		{
			printf("ERROR: Unable to open file: %s\n", path);
			//throw std::invalid_argument(std::string("Unable to open file: ") + path);
		}

		// Read the contents of the file into the temp variables
		while (true)
		{
			char lineHeader[128];
			// Read first word in line
			int res = fscanf_s(file, "%s", &lineHeader, (unsigned)_countof(lineHeader));
			if (res == EOF)
				break;
			else
			{
				// Deal with vertex coordinates
				if (strcmp(lineHeader, "v") == 0)
				{
					glm::vec3 vertex;
					fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					tempVertices.push_back(vertex);
				}

				// Deal with UV coordinates
				else if (strcmp(lineHeader, "vt") == 0)
				{
					glm::vec2 uv;
					fscanf_s(file, "%f %f\n", &uv.s, &uv.t);
					tempUVs.push_back(uv);
				}

				// Deal with normal coordinates
				else if (strcmp(lineHeader, "vn") == 0)
				{
					glm::vec3 normal;
					fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					tempNormals.push_back(normal);
				}

				// Deal with face indices
				else if (strcmp(lineHeader, "f") == 0)
				{
					// Should come in the form index 1, index 2, index 3
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
					int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
						&vertexIndex[0], &uvIndex[0], &normalIndex[0],
						&vertexIndex[1], &uvIndex[1], &normalIndex[1],
						&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
					if (matches != 9)
					{
						throw std::invalid_argument(path + std::string(" cannot be read by the parser. Try exporting with other options"));
					}
					for (int i = 0; i < 3; ++i)
					{
						vertexIndices.push_back(vertexIndex[i]);
						uvIndices.push_back(uvIndex[i]);
						normalIndices.push_back(normalIndex[i]);
					}
				}
			}

		}

		// Indexing
		for (unsigned int i = 0; i < vertexIndices.size(); ++i)
		{
			unsigned int locationIndex = vertexIndices[i] - 1;
			unsigned int uvIndex = uvIndices[i] - 1;
			unsigned int normalIndex = normalIndices[i] - 1;

			assert(locationIndex <= tempVertices.size() && tempVertices.size() != 0);
			assert(uvIndex <= tempUVs.size() && tempUVs.size() != 0);
			assert(normalIndex <= tempNormals.size() && tempNormals.size() != 0);

			glm::vec3 locationVertex = tempVertices.at(locationIndex);
			glm::vec2 uvVertex = tempUVs.at(uvIndex);
			glm::vec3 normalVertex = tempNormals.at(normalIndex);

			//new_shape.append_vertices({ locationVertex, DEFAULT_COLOUR, normalVertex, uvVertex });
			new_shape.append_vertex(Vertex(locationVertex, normalVertex, uvVertex));

		}

		new_shape.make_indices_smooth();
		new_shape.make_tangents();
		return new_shape;
	}
}