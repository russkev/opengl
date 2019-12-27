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
	// // ----- CONSTRUCTOR ----- // //
	Shader::Shader(const std::string& name, const char* vertexShader, const char* fragmentShader) :
		m_name{ name },
		m_programID{ LoadShaders::load(vertexShader, fragmentShader) }
	{
		fetchUniforms();
	}

	Shader::Shader(const std::string& name, const char* vertexShader, const char* geometryShader, const char* fragmentShader) :
		m_name{ name },
		m_programID{ LoadShaders::load(vertexShader, geometryShader, fragmentShader) }
	{
		fetchUniforms();
	}

	// // ----- GENERAL METHODS ----- // //

	// Tell opengl to use this shader for upcoming commands
	void Shader::use()
	{
		glUseProgram(m_programID);
	}

	bool Shader::containsUniform(std::string uniform_name)
	{
		return m_uniforms.find(uniform_name) != m_uniforms.end();
	}

	void Shader::updateLights(const std::vector<LightNode*>& light_nodes)
	{
		int index = 0;// , point_index = 0, directional_index = 0, spot_index = 0;
		std::string type = "";

		for (LightNode* light_node : light_nodes)
		{
			PointLight* point_light = dynamic_cast<PointLight*>(light_node->light());
			DirectionalLight* directional_light = dynamic_cast<DirectionalLight*>(light_node->light());
			SpotLight* spot_light = dynamic_cast<SpotLight*>(light_node->light());

			index = light_node->shaderIndex();
			type = light_node->light()->type();

			// Set uniforms
			if (point_light || spot_light)
			{
				setUniform(std::string(type + "[" + std::to_string(index) + "]." + LightNode::LIGHT_POSITION), light_node->worldPosition());
			}
			if (directional_light || spot_light)
			{
				setUniform(std::string(type + "[" + std::to_string(index) + "]." + LightNode::LIGHT_DIRECTION), light_node->directionVector());
			}
			if (spot_light)
			{
				setUniform(std::string(type + "[" + std::to_string(index) + "]." + SpotLight::INNER), spot_light->cos_innerAngle());
				setUniform(std::string(type + "[" + std::to_string(index) + "]." + SpotLight::OUTER), spot_light->cos_outerAngle());
			}
			setUniform(std::string(type + "[" + std::to_string(index) + "]." + Light::LIGHT_BRIGHTNESS), light_node->light()->brightness());
			setUniform(std::string(type + "[" + std::to_string(index) + "]." + Light::LIGHT_COLOR), light_node->light()->color());
		}
	}


	//Get all the uniforms from the shader and store their information with the shader
	void Shader::fetchUniforms()
	{
		use();
		int numUniforms;
		glGetProgramiv(m_programID, GL_ACTIVE_UNIFORMS, &numUniforms);
		for (int i = 0; i < numUniforms; ++i)
		{
			Uniform newUniform;
			char uniformNameChars[GL_ACTIVE_UNIFORM_MAX_LENGTH];
			GLsizei uniformNameLength;

			glGetActiveUniform(m_programID, i, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformNameLength, &newUniform.dataSize, &newUniform.type, uniformNameChars);
			newUniform.location = glGetUniformLocation(m_programID, uniformNameChars);

			if (newUniform.type == GL_SAMPLER_2D || newUniform.type == GL_SAMPLER_2D_ARRAY)
			{
				newUniform.texture_unit = m_num_textures;
				m_num_textures++;
			}

			std::string uniformNameString(uniformNameChars);

			m_uniforms[uniformNameString] = newUniform;
		}
	}

	// // ----- GETTERS ----- // //
	const GLuint Shader::programID() const
	{
		return m_programID;
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
	std::string& Shader::name()
	{
		return m_name;
	}
} // namespace gl_engine