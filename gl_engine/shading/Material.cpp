#include "Material.h"

Material::Material(const std::string name, const char* vertexShader, const char* fragmentShader) :
	m_shader(Shader(name, vertexShader, fragmentShader))
{}

Shader& Material::shader()
{
	return m_shader;
}
