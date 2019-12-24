#include "Material.h"

#include "Texture.h"

namespace gl_engine
{
	Material::Material(const std::string name, const char* vertexShader, const char* fragmentShader) :
		Shader(name, vertexShader, fragmentShader)
	{}

	void Material::addTexture(const std::string& uniform_name, Texture* texture)
	{
		if (m_textures.find(uniform_name) != m_textures.end())
		{
			printf("WARNING: \"%s\" is overwriting an existing texture\n", uniform_name.c_str());
		}
		m_textures[uniform_name] = texture;
		Shader::setTexture(uniform_name, *texture);
	}

	void Material::bindTextures()
	{
		int texture_unit = 0;
		for (const auto & texture_pair : m_textures)
		{
			std::string curr_locationName = texture_pair.first;
			Texture* curr_texture = texture_pair.second;
			//auto texture_unit = Shader::uniforms().at(curr_locationName).texture_unit;

			Shader::setUniform(curr_locationName, texture_unit);
			curr_texture->bind(texture_unit);
			texture_unit++;
		}
	}

}  // namespace gl_engine

