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
		for (const auto & texture_pair : m_textures)
		{
			bind_texture(texture_pair.first, texture_pair.second);
		}
	}

	void Material::bind_texture(const std::string uniform_name, Texture* texture)
	{
		if (Shader::uniforms().find(uniform_name) != Shader::uniforms().end())
		{
			auto texture_unit = Shader::uniforms().at(uniform_name).texture_unit;
			Shader::set_uniform(uniform_name, texture_unit);
			texture->bind(texture_unit);
		}
	}

	void Material::unbind_textures()
	{
		for (const auto & texture_pair : m_textures)
		{
			texture_pair.second->unbind();
		}
	}

	void Material::update_texture_id(std::string uniform_name, const GLuint id)
	{
		Texture* texture = m_textures[uniform_name];
		//Shader::use();
		texture->set_new_id(id);
		texture->bind();
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

