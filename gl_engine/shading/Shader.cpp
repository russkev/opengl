#include "Shader.h"
#include "LoadShader.h"
#include <glm/glm.hpp>

Shader::Shader(const std::string& name, const char* vertexShader, const char* fragmentShader) :
	m_name(name)
{
	m_programID = LoadShaders::load(vertexShader, fragmentShader);
	
	getUniforms();
	auto sdfs = 345;
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

// // ----- SETTERS ----- // //
std::string& Shader::name()
{
	return m_name;
}

// // ----- GENERAL METHODS ----- // //
void Shader::use()
{
	glUseProgram(m_programID);
}


/*

	Get all the uniforms from the shader and store their information with the shader

*/
void Shader::getUniforms()
{
	int numUniforms;
	glGetProgramiv(m_programID, GL_ACTIVE_UNIFORMS, &numUniforms);
	for (int i = 0; i < numUniforms; ++i)
	{
		Uniform newUniform;
		char uniformNameChars[GL_ACTIVE_UNIFORM_MAX_LENGTH];
		GLsizei uniformNameLength;

		glGetActiveUniform(m_programID, i, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformNameLength, &newUniform.dataSize, &newUniform.type, uniformNameChars);

		std::string uniformNameString(uniformNameChars);

		m_uniforms[uniformNameString] = newUniform;
	}
}