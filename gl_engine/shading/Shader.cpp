#include "../node/LightNode.h"
#include "../light/PointLight.h"

#include "Shader.h"
#include "LoadShader.h"
#include <glm/glm.hpp>

// // ----- CONSTRUCTOR ----- // //
Shader::Shader(const std::string& name, const char* vertexShader, const char* fragmentShader) :
	m_name(name)
{
	m_programID = LoadShaders::load(vertexShader, fragmentShader);
	fetchUniforms();
}

// // ----- GENERAL METHODS ----- // //
void Shader::setTexture(const std::string& location_name, Texture& texture)
{
	Uniform* thisUniform;
	
	use();
	thisUniform = &m_uniforms.at(location_name);

	GLuint tex_id;
	GLint tex_loc;
	GLenum mode = texture.hasAlpha() ? GL_RGBA : GL_RGB;

	glGenTextures(1, &tex_id);

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
		GL_RGB,
		GL_UNSIGNED_BYTE,
		texture.data()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);

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
	int point_index = 0;
	for (LightNode* light_node : light_nodes)
	{
		if (PointLight* derived_light = dynamic_cast<PointLight*>(light_node->light()))
		{
			setUniform("point_light[" + std::to_string(point_index) + "].position", light_node->worldPosition());
			setUniform("point_light[" + std::to_string(point_index) + "].brightness", derived_light->brightness());
			setUniform("point_light[" + std::to_string(point_index) + "].color", derived_light->brightness());
			point_index++;
		}
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

