#include "pch.h"
#include "Material.h"

#include "node/LightNode.h"
#include "light/PointLight.h"
#include "light/DirectionalLight.h"
#include "light/SpotLight.h"
#include "light/Light.h"
#include "shading/LoadShader.h"

#include <glm/glm.hpp>

namespace glen
{
	// // ----- CONSTRUCTORS ----- // //
	Material::Material(const std::string& name, const char* vertex_shader, const char* fragment_shader) :
		m_name{ name },
		m_program_id{ LoadShaders::load(vertex_shader, fragment_shader) }
	{
		init();
	}

	Material::Material(const std::string& name, const char* vertex_shader, const char* geometry_shader, const char* fragment_shader) :
		m_name{ name },
		m_program_id{ LoadShaders::load(vertex_shader, geometry_shader, fragment_shader) }
	{
		init();
	}

	Material::~Material()
	{
		glDeleteProgram(m_program_id);
	}

	// // ----- GENERAL METHODS ----- // //

	// Tell opengl to use this shader for upcoming commands
	void Material::use()
	{
		glUseProgram(m_program_id);
	}

	bool Material::is_uniform(const GLenum type)
	{
		return (
			ShaderType::gl_float_samplerTypes.find(type) != ShaderType::gl_float_samplerTypes.end() ||
			ShaderType::gl_int_samplerTypes.find(type) != ShaderType::gl_int_samplerTypes.end() ||
			ShaderType::gl_uint_samplerTypes.find(type) != ShaderType::gl_uint_samplerTypes.end()
			);
	}


	bool Material::contains_uniform(std::string uniform_name)
	{
		return m_uniforms.find(uniform_name) != m_uniforms.end();
	}

	void Material::bind_textures()
	{
		for (const auto & texture_pair : m_textures)
		{
			bind_texture(texture_pair.first, texture_pair.second);
		}
	}

	void Material::bind_texture(const std::string uniform_name, Texture* texture)
	{
		if (Material::uniforms().find(uniform_name) != Material::uniforms().end())
		{
			auto texture_unit = Material::uniforms().at(uniform_name).texture_unit;
			Material::set_uniform(uniform_name, texture_unit);
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
		texture->set_new_id(id);
		texture->bind();
	}

	void Material::init()
	{
		fetch_uniforms();
	}
	//Get all the uniforms from the shader and store their information with the shader
	void Material::fetch_uniforms()
	{
		use();
		int numUniforms;
		glGetProgramiv(m_program_id, GL_ACTIVE_UNIFORMS, &numUniforms);
		for (int i = 0; i < numUniforms; ++i)
		{
			Uniform newUniform;
			char uniformNameChars[GL_ACTIVE_UNIFORM_MAX_LENGTH];
			GLsizei uniformNameLength;

			glGetActiveUniform(m_program_id, i, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformNameLength, &newUniform.data_size, &newUniform.type, uniformNameChars);
			newUniform.location = glGetUniformLocation(m_program_id, uniformNameChars);

			if (is_uniform(newUniform.type))
			{
				newUniform.texture_unit = m_num_textures;
				m_num_textures++;
			}

			std::string uniformNameString(uniformNameChars);

			m_uniforms[uniformNameString] = newUniform;
		}
	}

	// // ----- GETTERS ----- // //
	const GLuint Material::program_id() const
	{
		return m_program_id;
	}

	const std::string Material::name() const
	{
		return m_name;
	}

	const std::map<std::string, Uniform>& Material::uniforms() const
	{
		return m_uniforms;
	}

	// // ----- SETTERS ----- // //
	void Material::set_name(const std::string& name)
	{
		m_name = name;
	}

	void Material::set_sampler_color(const std::string& uniform_name, glm::vec3& color)
	{
		//Texture texture{ color };
		//m_colors.push_back(std::move(texture));
		m_colors.emplace_back(Texture{ color });
		set_texture(uniform_name, &m_colors.back());
	}

	void Material::set_sampler_value(const std::string& uniform_name, GLfloat value)
	{
		glm::vec3 color_value{ value };
		set_sampler_color(uniform_name, color_value);
	}

	void Material::set_texture(const std::string& uniform_name, Texture* texture)
	{
		if (Material::uniforms().find(uniform_name) == Material::uniforms().end())
		{
			if (m_failed_textures.find(uniform_name) == m_failed_textures.end())
			{
				printf("WARNING: Unable to set texture: \"%s\" for shader: \"%s\", uniform not found\n", uniform_name.c_str(), Material::name().c_str());
				m_failed_textures.insert(uniform_name);
				return;
			}
		}

		m_textures[uniform_name] = texture;
		texture->process();
	}
}