#include "../node/LightNode.h"
#include "../light/PointLight.h"
#include "../light/DirectionalLight.h"
#include "../light/SpotLight.h"

#include "Shader.h"
#include "LoadShader.h"
#include <glm/glm.hpp>

namespace gl_engine
{
	// // ----- CONSTRUCTOR ----- // //
	Shader::Shader(const std::string& name, const char* vertexShader, const char* fragmentShader) :
		m_name(name)
	{
		m_programID = LoadShaders::load(vertexShader, fragmentShader);
		fetchUniforms();
	}

	// // ----- GENERAL METHODS ----- // //
	GLuint Shader::setTexture(const std::string& location_name, Texture& texture, GLenum mode, GLenum type)
	{
		if (m_uniforms.find(location_name) == m_uniforms.end())
		{
			printf("WARNING: Unable to set texture: \"%s\", uniform not found\n", location_name.c_str());
			return 0;
		}

		Uniform* thisUniform;

		//use();
		thisUniform = &m_uniforms.at(location_name);

		GLint tex_loc;
		GLenum tex_mode = mode == GL_RGBA && !texture.hasAlpha() ? GL_RGB : mode;

		tex_loc = thisUniform->location;
		glUniform1i(tex_loc, (GLint)texture.tex_id());

		glActiveTexture(GLenum(GL_TEXTURE0 + texture.tex_id()));

		glBindTexture(GL_TEXTURE_2D, (GLuint)texture.tex_id());

		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			mode,
			(GLsizei)texture.width(),
			(GLsizei)texture.height(),
			0,
			tex_mode,
			type,
			texture.data()
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		//return tex_id;
		return 0;
	}


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
		int index = 0, point_index = 0, directional_index = 0, spot_index = 0;
		std::string type = "";

		for (LightNode* light_node : light_nodes)
		{
			PointLight* point_light = dynamic_cast<PointLight*>(light_node->light());
			DirectionalLight* directional_light = dynamic_cast<DirectionalLight*>(light_node->light());
			SpotLight* spot_light = dynamic_cast<SpotLight*>(light_node->light());

			// Adjust indices
			if (point_light)
			{
				type = point_light->type();
				index = point_index;
				point_index++;
			}
			if (directional_light)
			{
				type = directional_light->type();
				index = directional_index;
				directional_index++;
			}
			if (spot_light)
			{
				type = spot_light->type();
				index = spot_index;
				spot_index++;
			}

			// Set uniforms
			if (point_light || spot_light)
			{
				setUniform(std::string(type + "[" + std::to_string(index) + "]" + LightNode::LIGHT_POSITION), light_node->worldPosition());
			}
			if (directional_light || spot_light)
			{
				setUniform(std::string(type + "[" + std::to_string(index) + "]" + LightNode::LIGHT_DIRECTION), light_node->directionVector());
			}
			if (spot_light)
			{
				setUniform(std::string(type + "[" + std::to_string(index) + "]" + SpotLight::INNER), spot_light->cos_innerAngle());
				setUniform(std::string(type + "[" + std::to_string(index) + "]" + SpotLight::OUTER), spot_light->cos_outerAngle());
			}
			setUniform(std::string(type + "[" + std::to_string(index) + "]" + Light::LIGHT_BRIGHTNESS), light_node->light()->brightness());
			setUniform(std::string(type + "[" + std::to_string(index) + "]" + Light::LIGHT_COLOR), light_node->light()->color());
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