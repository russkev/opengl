#include "Mesh.h"

#include <stdexcept>
#include <glm/matrix.hpp>
#include <cassert>

struct OBJ_Loader
{
	// Set constants
	const static glm::vec3 DEFAULT_COLOUR;

	// Static methods
	static Mesh load_obj(const char *path);

};