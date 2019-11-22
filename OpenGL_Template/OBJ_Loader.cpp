#include "OBJ_Loader.h"

bool OBJ_Loader::load_obj(const char *path)
{
	// Create temp variables
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	ShapeData new_shape;

	// Attempt to open file
	FILE * file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Unable to open file: %s\n", path);
		return false;
	}

	// Read the contents of the file into the temp variables
	while (true)
	{
		char lineHeader[128];
		// Read first line in file
		int res = fscanf(file, "&s", lineHeader);
		if (res == EOF)
			break;
		else
		{
			// Compare the beginning of the line to the chars
			if (strcmp(lineHeader, "v ") == 0)
			{
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.s, &uv.t);
				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0)
			{
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d %d/%d/%d\n",
					vertexIndex[0], vertexIndex[1], vertexIndex[2],
					uvIndex[0], uvIndex[1], uvIndex[2],
					normalIndex[0], normalIndex[1], normalIndex[2]);
				if (matches != 9)
				{
					printf("%s cannot be read by the parser. Try exporting with other options");
					return false;
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
	for (unsigned int i = 0; i < vertexIndices.size; ++i)
	{
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		ShapeData.
	}

	
	new_shape.append_vertices({ glm::vec3(+1.0f, +1.0f, +1.0f), glm::vec3(+1.0f, +1.0f, +1.0f), glm::vec3(+1.0f, +1.0f, +1.0f), glm::vec2(0.0f, 0.0f) });

}