#include "Material.h"

namespace gl_engine
{
	Material::Material(const std::string name, const char* vertexShader, const char* fragmentShader) :
		Shader(name, vertexShader, fragmentShader)
	{}
}  // namespace gl_engine
