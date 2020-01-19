#include "Material.h"

#include "Texture.h"

namespace gl_engine
{
	// // ----- CONSTRUCTORS ----- // //
	Material::Material(const std::string name, const char* vertex_shader, const char* fragment_shader) :
		Shader{ name, vertex_shader, fragment_shader }
	{}

	Material::Material(const std::string name, const char* vertexShader, const char* geometryShader, const char* fragmentShader) :
		Shader{ name, vertexShader, geometryShader, fragmentShader }
	{}

	// // ----- GENERAL METHODS ----- // //
	void Material::bind_textures()
	{
		//int texture_unit = 0;
		for (const auto & texture_pair : m_textures)
		{
			std::string curr_locationName = texture_pair.first;
			if (Shader::uniforms().find(curr_locationName) != Shader::uniforms().end())
			{
				Texture* curr_texture = texture_pair.second;
				auto texture_unit = Shader::uniforms().at(curr_locationName).texture_unit;

				Shader::set_uniform(curr_locationName, texture_unit);
				curr_texture->bind(texture_unit);
			}
		}
	}

	void Material::unbind_textures()
	{
		for (const auto & texture_pair : m_textures)
		{
			texture_pair.second->unbind();
		}
	}

	// // ----- SETTERS ----- // //
	void Material::set_sampler_color(const std::string& uniform_name, glm::vec3& color)
	{
		Texture texture{ color };
		m_colors.push_back(texture);
		set_texture(uniform_name, &m_colors.back());
	}

	void Material::set_sampler_value(const std::string& uniform_name, GLfloat value)
	{
		set_sampler_color(uniform_name, glm::vec3(value));
	}

	void Material::set_texture(const std::string& uniform_name, Texture* texture)
	{
		if (Shader::uniforms().find(uniform_name) == Shader::uniforms().end())
		{
			if (m_failed_uniforms.find(uniform_name) == m_failed_uniforms.end())
			{
				printf("WARNING: Unable to set texture: \"%s\" for shader: \"%s\", uniform not found\n", uniform_name.c_str(), Shader::name().c_str());
				m_failed_uniforms.insert(uniform_name);
				return;
			}
		}

		m_textures[uniform_name] = texture;
		texture->process();
	}

}  // namespace gl_engine

