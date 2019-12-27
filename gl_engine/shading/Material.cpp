#include "Material.h"

#include "Texture.h"

namespace gl_engine
{
	Material::Material(const std::string name, const char* vertexShader, const char* fragmentShader) :
		Shader{ name, vertexShader, fragmentShader }
	{}

	Material::Material(const std::string name, const char* vertexShader, const char* geometryShader, const char* fragmentShader) :
		Shader{ name, vertexShader, geometryShader, fragmentShader }
	{}

	void Material::addTexture(const std::string& uniform_name, Texture* texture)
	{
		if (m_textures.find(uniform_name) != m_textures.end())
		{
			printf("WARNING: \"%s\" is overwriting an existing texture\n", uniform_name.c_str());
		}

		if (Shader::uniforms().find(uniform_name) == Shader::uniforms().end())
		{
			printf("WARNING: Unable to set texture: \"%s\" for shader: \"%s\", uniform not found\n", uniform_name.c_str(), Shader::name().c_str());
			return;
		}

		m_textures[uniform_name] = texture;
		texture->process();
	}

	void Material::bindTextures()
	{
		//int texture_unit = 0;
		for (const auto & texture_pair : m_textures)
		{
			std::string curr_locationName = texture_pair.first;
			Texture* curr_texture = texture_pair.second;
			auto texture_unit = Shader::uniforms().at(curr_locationName).texture_unit;

			Shader::setUniform(curr_locationName, texture_unit);
			curr_texture->bind(texture_unit);
		}
	}

	void Material::unbindTextures()
	{
		for (const auto & texture_pair : m_textures)
		{
			texture_pair.second->unbind();
		}
	}

}  // namespace gl_engine

