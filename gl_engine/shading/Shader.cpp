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
// Tell opengl to use this shader for upcoming commands
void Shader::use()
{
	glUseProgram(m_programID);
}


//Get all the uniforms from the shader and store their information with the shader
void Shader::fetchUniforms()
{
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

