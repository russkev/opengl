#include "../node/LightNode.h"
#include "../light/PointLight.h"
#include "../light/DirectionalLight.h"
#include "../light/SpotLight.h"
#include "../light/Light.h"

#include "Shader.h"
#include "LoadShader.h"
#include <glm/glm.hpp>

namespace gl_engine
{
	// // ----- CONSTRUCTORS ----- // //
	Shader::Shader(const std::string& name, const char* vertex_shader, const char* fragment_shader) :
		m_name{ name },
		m_program_id{ LoadShaders::load(vertex_shader, fragment_shader) }
	{
		init();
	}

	Shader::Shader(const std::string& name, const char* vertex_shader, const char* geometry_shader, const char* fragment_shader) :
		m_name{ name },
		m_program_id{ LoadShaders::load(vertex_shader, geometry_shader, fragment_shader) }
	{
		init();
	}

	// // ----- GENERAL METHODS ----- // //

	// Tell opengl to use this shader for upcoming commands
	void Shader::use()
	{
		glUseProgram(m_program_id);
	}

	bool Shader::is_uniform(const GLenum type)
	{
		return (
			ShaderType::gl_float_samplerTypes.find(type) != ShaderType::gl_float_samplerTypes.end() ||
			ShaderType::gl_int_samplerTypes.find(type) != ShaderType::gl_int_samplerTypes.end() ||
			ShaderType::gl_uint_samplerTypes.find(type) != ShaderType::gl_uint_samplerTypes.end()
			);
	}


	bool Shader::contains_uniform(std::string uniform_name)
	{
		return m_uniforms.find(uniform_name) != m_uniforms.end();
	}

	void Shader::update_lights(const std::vector<LightNode*>& light_nodes)
	{
		int index = 0;// , point_index = 0, directional_index = 0, spot_index = 0;
		std::string type = "";

		for (LightNode* light_node : light_nodes)
		{
			PointLight* pointLight = dynamic_cast<PointLight*>(light_node->light());
			DirectionalLight* directionalLight = dynamic_cast<DirectionalLight*>(light_node->light());
			SpotLight* spotLight = dynamic_cast<SpotLight*>(light_node->light());

			index = light_node->shader_pos();
			type = light_node->light()->type();

			// Set uniforms
			if (pointLight || spotLight)
			{
				set_uniform(std::string(type + "[" + std::to_string(index) + "]." + LightNode::LIGHT_POSITION), light_node->world_position());
			}
			if (directionalLight || spotLight)
			{
				set_uniform(std::string(type + "[" + std::to_string(index) + "]." + LightNode::LIGHT_DIRECTION), light_node->direction());
			}
			if (spotLight)
			{
				set_uniform(std::string(type + "[" + std::to_string(index) + "]." + SpotLight::INNER), spotLight->cos_inner_angle());
				set_uniform(std::string(type + "[" + std::to_string(index) + "]." + SpotLight::OUTER), spotLight->cos_outer_angle());
			}
			set_uniform(std::string(type + "[" + std::to_string(index) + "]." + Light::LIGHT_BRIGHTNESS), light_node->light()->brightness());
			set_uniform(std::string(type + "[" + std::to_string(index) + "]." + Light::LIGHT_COLOR), light_node->light()->color());
		}
	}

	void Shader::init()
	{
		//std::printf("Shader \"%s\" loaded.\n", m_name.c_str());
		fetch_uniforms();
	}
	//Get all the uniforms from the shader and store their information with the shader
	void Shader::fetch_uniforms()
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
	const GLuint Shader::program_id() const
	{
		return m_program_id;
	}

	const std::string Shader::name() const
	{
		return m_name;
	}

	const std::map<std::string, Uniform>& Shader::uniforms() const
	{
		return m_uniforms;
	}

	// // ----- SETTERS ----- // //
	void Shader::set_name(const std::string& name)
	{
		m_name = name;
	}
} // namespace gl_engine